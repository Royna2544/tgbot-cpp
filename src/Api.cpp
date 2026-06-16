#include <tgbot/Api.h>
#include <tgbot/Logger.h>
#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>
#include <tgbot/net/HttpReqArg.h>
#include <tgbot/tools/StringTools.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <variant>

#include "tgbot/net/HttpClient.h"
#include "tgbot/types/InputFile.h"
#include "tgbot/types/Update.h"

namespace detail {

using namespace TgBot::detail;

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type {
    using type = T;
};

template <typename T, typename V, V min, V max, V defaults>
struct is_optional<TgBot::Api::bounded_optional_default<T, min, max, defaults>>
    : std::true_type {
    using type = T;
};

template <typename T, T defaults>
struct is_optional<TgBot::Api::optional_default<T, defaults>> : std::true_type {
    using type = T;
};

template <typename T, typename V, V min, V max>
struct is_optional<TgBot::Api::bounded_optional<T, min, max>> : std::true_type {
    using type = T;
};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

template <typename T>
using optional_t = typename is_optional<T>::type;

template <typename... Args>
struct is_variant : std::false_type {};

template <typename... Args>
struct is_variant<std::variant<Args...>> : std::true_type {};

template <typename T>
constexpr bool is_variant_v = is_variant<T>::value;

template <typename T>
struct is_vector : std::false_type {};

template <typename T>
struct is_vector<std::vector<T>> : std::true_type {
    using type = T;
};

template <typename T>
constexpr bool is_vector_v = is_vector<T>::value;

}  // namespace detail

namespace {

// Specialized handling for generic T
template <typename T>
auto putArg(std::string name, const T& data) {
    if constexpr (std::is_same_v<T, std::string_view> ||
                  detail::is_primitive_v<T>) {
        return std::make_unique<TgBot::HttpReqArg>(std::move(name), data);
    } else if constexpr (std::is_same_v<
                             T, std::chrono::system_clock::time_point>) {
        return std::make_unique<TgBot::HttpReqArg>(
            std::move(name), std::chrono::system_clock::to_time_t(data));
    } else if constexpr (std::is_same_v<T, TgBot::InputFile::Ptr>) {
        return std::make_unique<TgBot::HttpReqArgFile>(std::move(name), data);
    } else if constexpr (detail::is_variant_v<T>) {
        // e.g. an optional<ChatIdType> that was unwrapped to its variant
        return std::visit([&name](const auto& v) { return putArg(name, v); },
                          data);
    } else {
        return std::make_unique<TgBot::HttpReqArg>(std::move(name),
                                                   TgBot::putJSON(data));
    }
}

using TgBot::TgException;

template <typename... Args>
nlohmann::json sendRequest(const std::string_view _bot_url,
                           TgBot::HttpClient* _httpClient,
                           const std::string_view method,
                           std::pair<const char*, Args>&&... args) {
    TgBot::HttpReqArg::Vec vec;
    std::string url(_bot_url);
    url += method;

    vec.reserve(sizeof...(Args));
    (
        [&vec](const std::pair<const char*, Args> arg) {
            using T = std::decay_t<Args>;
            if constexpr (detail::is_optional_v<T>) {
                if (static_cast<bool>(arg.second)) {
                    vec.emplace_back(putArg(arg.first, *arg.second));
                }
            } else if constexpr (detail::is_variant_v<T>) {
                if (arg.second.index() != std::variant_npos) {
                    std::visit(
                        [&](const auto& v) {
                            using T = std::decay_t<decltype(v)>;
                            if constexpr (detail::is_shared_ptr_v<T>) {
                                if (v != nullptr) {
                                    vec.emplace_back(putArg(arg.first, v));
                                }
                            } else {
                                vec.emplace_back(putArg(arg.first, v));
                            }
                        },
                        arg.second);
                }
            } else if constexpr (detail::is_shared_ptr_v<T>) {
                if (arg.second != nullptr) {
                    vec.emplace_back(putArg(arg.first, arg.second));
                }
            } else if constexpr (detail::is_vector_v<T>) {
                if (arg.second.size() != 0) {
                    vec.emplace_back(putArg(arg.first, arg.second));
                }
            } else {
                vec.emplace_back(putArg(arg.first, arg.second));
            }
        }(args),
        ...);

    int retries = 0;
    {
        std::ostringstream trace;
        trace << "Sending request: " << method;
        for (const auto& arg : vec) {
            trace << "\n  ";
            arg->print(trace);
        }
        TgBot::detail::log(TgBot::LogLevel::Trace, trace.str());
    }
    constexpr int max_retries = TgBot::HttpClient::kRequestMaxRetries;
    while (true) {
        try {
            std::string serverResponse = _httpClient->makeRequest(url, vec);

            if (!serverResponse.compare(0, 6, "<html>")) {
                throw TgException(
                    "tgbot-cpp library have got html page instead of json "
                    "response. Maybe you entered wrong bot token.",
                    TgException::ErrorCode::HtmlResponse);
            }

            nlohmann::json result;
            try {
                result = nlohmann::json::parse(serverResponse);
            } catch (const nlohmann::json::parse_error&) {
                TgBot::detail::log(
                    TgBot::LogLevel::Error,
                    "Failed to parse json response: " + serverResponse);
                throw TgException(
                    "tgbot-cpp library can't parse json response.",
                    TgException::ErrorCode::InvalidJson);
            }

            if (result.value("ok", false)) {
                return result["result"];
            }

            const std::string message =
                result.value("description", "Unknown error");
            const int errorCode = result.value("error_code", 0);

            // Honour Telegram rate limiting (HTTP 429): the request never
            // reached the bot logic, so wait the suggested time and retry.
            if (errorCode == 429 &&
                (max_retries < 0 || retries < max_retries)) {
                std::chrono::seconds delay = TgBot::HttpClient::kRequestBackoff;
                if (result.contains("parameters") &&
                    result["parameters"].contains("retry_after")) {
                    delay = std::chrono::seconds(
                        result["parameters"]["retry_after"].get<int>());
                }
                TgBot::detail::log(
                    TgBot::LogLevel::Warning,
                    "Rate limited by Telegram, retrying " + std::string(method) +
                        " after " + std::to_string(delay.count()) + "s");
                std::this_thread::sleep_for(delay);
                retries++;
                continue;
            }

            throw TgException(message,
                              static_cast<TgException::ErrorCode>(errorCode));
        } catch (const TgBot::NetworkException& ex) {
            // Only transient network failures are retried; API errors above are
            // deterministic and must propagate (retrying could duplicate
            // non-idempotent requests such as sendMessage).
            const bool willRetry = max_retries < 0 || retries < max_retries;
            TgBot::detail::log(
                willRetry ? TgBot::LogLevel::Warning : TgBot::LogLevel::Error,
                std::string("Network error on ") + std::string(method) + ": " +
                    ex.what() + (willRetry ? " (retrying)" : " (giving up)"));
            if (!willRetry) {
                throw;
            }
            std::this_thread::sleep_for(TgBot::HttpClient::kRequestBackoff);
            retries++;
        }
    }
}

}  // namespace

namespace TgBot {

template <>
std::string putJSON<TgBot::Update::Types>(const TgBot::Update::Types& object) {
    nlohmann::json json = nlohmann::json::array();
    if (object & Update::Types::business_connection) {
        json.push_back("business_connection");
    }
    if (object & Update::Types::edited_business_message) {
        json.push_back("edited_business_message");
    }
    if (object & Update::Types::edited_channel_post) {
        json.push_back("edited_channel_post");
    }
    if (object & Update::Types::edited_message) {
        json.push_back("edited_message");
    }
    if (object & Update::Types::message) {
        json.push_back("message");
    }
    if (object & Update::Types::channel_post) {
        json.push_back("channel_post");
    }
    if (object & Update::Types::business_message) {
        json.push_back("business_message");
    }
    if (object & Update::Types::deleted_business_messages) {
        json.push_back("deleted_business_messages");
    }
    if (object & Update::Types::inline_query) {
        json.push_back("inline_query");
    }
    if (object & Update::Types::poll) {
        json.push_back("poll");
    }
    if (object & Update::Types::shipping_query) {
        json.push_back("shipping_query");
    }
    if (object & Update::Types::chosen_inline_result) {
        json.push_back("chosen_inline_result");
    }
    if (object & Update::Types::callback_query) {
        json.push_back("callback_query");
    }
    if (object & Update::Types::poll_answer) {
        json.push_back("poll_answer");
    }
    if (object & Update::Types::message_reaction) {
        json.push_back("message_reaction");
    }
    if (object & Update::Types::message_reaction_count) {
        json.push_back("message_reaction_count");
    }
    if (object & Update::Types::my_chat_member) {
        json.push_back("my_chat_member");
    }
    if (object & Update::Types::chat_member) {
        json.push_back("chat_member");
    }
    if (object & Update::Types::chat_join_request) {
        json.push_back("chat_join_request");
    }
    if (object & Update::Types::chat_boost) {
        json.push_back("chat_boost");
    }
    if (object & Update::Types::removed_chat_boost) {
        json.push_back("removed_chat_boost");
    }
    if (object & Update::Types::pre_checkout_query) {
        json.push_back("pre_checkout_query");
    }
    if (object & Update::Types::purchased_paid_media) {
        json.push_back("purchased_paid_media");
    }
    return json.dump();
}

template <>
std::string putJSON<Api::ParseMode>(const Api::ParseMode& object) {
    switch (object) {
        case Api::ParseMode::Markdown:
            return "Markdown";
        case Api::ParseMode::MarkdownV2:
            return "MarkdownV2";
        case Api::ParseMode::HTML:
            return "HTML";
        case Api::ParseMode::None:
            return "";
    }
    return {};
}

template <>
std::string putJSON<Api::PollType>(const Api::PollType& object) {
    switch (object) {
        case Api::PollType::regular:
            return "regular";
        case Api::PollType::quiz:
            return "quiz";
    }
    return {};
}

template <>
std::string putJSON<Api::StickerFormat>(const Api::StickerFormat& object) {
    switch (object) {
        case Api::StickerFormat::Static:
            return "static";
        case Api::StickerFormat::Animated:
            return "animated";
        case Api::StickerFormat::Video:
            return "video";
    }
    return {};
}

template <>
std::string putJSON<Api::ChatAction>(const Api::ChatAction& object) {
    switch (object) {
        case Api::ChatAction::typing:
            return "typing";
        case Api::ChatAction::upload_photo:
            return "upload_photo";
        case Api::ChatAction::record_video:
            return "record_video";
        case Api::ChatAction::upload_video:
            return "upload_video";
        case Api::ChatAction::record_voice:
            return "record_voice";
        case Api::ChatAction::upload_voice:
            return "upload_voice";
        case Api::ChatAction::upload_document:
            return "upload_document";
        case Api::ChatAction::find_location:
            return "find_location";
        case Api::ChatAction::record_video_note:
            return "record_video_note";
        case Api::ChatAction::upload_video_note:
            return "upload_video_note";
        case Api::ChatAction::choose_sticker:
            return "choose_sticker";
    }
    return {};
}

template <>
std::string putJSON<Sticker::Type>(const Sticker::Type& object) {
    switch (object) {
        case Sticker::Type::Regular:
            return "regular";
        case Sticker::Type::Mask:
            return "mask";
        case Sticker::Type::CustomEmoji:
            return "custom_emoji";
    }
    return {};
}

Api::Api(std::string token, HttpClient* httpClient, std::string url)
    : _bot_api_baseurl(url + "/bot" + token + "/"),
      _token(std::move(token)),
      _url(std::move(url)),
      _httpClient(httpClient) {}

std::vector<Update::Ptr> Api::getUpdates(
    optional<std::int32_t> offset,
    bounded_optional_default<std::int32_t, 0, 100, 100> limit,
    optional_default<std::int32_t, 0> timeout,
    const optional<Update::Types> allowedUpdates) const {
    return parseArray<Update>(
        sendRequest(_bot_api_baseurl, _httpClient, "getUpdates",
                    std::pair{"offset", offset}, std::pair{"limit", limit},
                    std::pair{"timeout", timeout},
                    std::pair{"allowed_updates", allowedUpdates}));
}

bool Api::setWebhook(
    const std::string_view url, InputFile::Ptr certificate,
    bounded_optional_default<std::int32_t, 1, 100, 40> maxConnections,
    const optional<Update::Types> allowedUpdates,
    const optional<std::string_view> ipAddress,
    optional<bool> dropPendingUpdates,
    const optional<std::string_view> secretToken) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setWebhook",
                       std::pair{"url", url},
                       std::pair{"certificate", std::move(certificate)},
                       std::pair{"max_connections", maxConnections},
                       std::pair{"allowed_updates", allowedUpdates},
                       std::pair{"ip_address", ipAddress},
                       std::pair{"drop_pending_updates", dropPendingUpdates},
                       std::pair{"secret_token", secretToken})
        .get<bool>();
}

bool Api::deleteWebhook(optional<bool> dropPendingUpdates) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteWebhook",
                       std::pair{"drop_pending_updates", dropPendingUpdates})
        .get<bool>();
}

WebhookInfo::Ptr Api::getWebhookInfo() const {
    const auto& p =
        sendRequest(_bot_api_baseurl, _httpClient, "getWebhookInfo");

    if (!p.contains("url")) {
        return nullptr;
    }

    if (!p["url"].get<std::string>().empty()) {
        return parse<WebhookInfo>(p);
    } else {
        return nullptr;
    }
}

User::Ptr Api::getMe() const {
    return parse<User>(sendRequest(_bot_api_baseurl, _httpClient, "getMe"));
}

bool Api::logOut() const {
    return sendRequest(_bot_api_baseurl, _httpClient, "logOut").get<bool>();
}

bool Api::close() const {
    return sendRequest(_bot_api_baseurl, _httpClient, "close").get<bool>();
}

Message::Ptr Api::sendMessage(
    ChatIdType chatId, const std::string_view text,
    LinkPreviewOptions::Ptr linkPreviewOptions,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr>& entities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendMessage",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"text", text},
        std::pair{"parse_mode", parseMode},
        std::pair{"disable_notification", disableNotification},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"entities", entities},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)},
        std::pair{"link_preview_options", std::move(linkPreviewOptions)}));
}

Message::Ptr Api::forwardMessage(
    ChatIdType chatId, ChatIdType fromChatId, std::int32_t messageId,
    optional<bool> disableNotification, optional<bool> protectContent,
    optional<std::int32_t> messageThreadId,
    optional<std::int32_t> directMessagesTopicId,
    optional<std::int32_t> videoStartTimestamp,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "forwardMessage",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"from_chat_id", std::move(fromChatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"protect_content", protectContent},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"direct_messages_topic_id", directMessagesTopicId},
                    std::pair{"video_start_timestamp", videoStartTimestamp},
                    std::pair{"message_effect_id", messageEffectId},
                    std::pair{"suggested_post_parameters",
                              std::move(suggestedPostParameters)}));
}

std::vector<MessageId::Ptr> Api::forwardMessages(
    ChatIdType chatId, ChatIdType fromChatId,
    const std::vector<std::int32_t>& messageIds,
    optional<std::int32_t> messageThreadId, optional<bool> disableNotification,
    optional<bool> protectContent,
    optional<std::int32_t> directMessagesTopicId) const {
    return parseArray<MessageId>(
        sendRequest(_bot_api_baseurl, _httpClient, "forwardMessages",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"from_chat_id", std::move(fromChatId)},
                    std::pair{"message_ids", messageIds},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"protect_content", protectContent},
                    std::pair{"direct_messages_topic_id",
                              directMessagesTopicId}));
}

MessageId::Ptr Api::copyMessage(
    ChatIdType chatId, ChatIdType fromChatId, std::int32_t messageId,
    const optional<std::string_view> caption,
    const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    GenericReply::Ptr replyMarkup, optional<bool> protectContent,
    optional<std::int32_t> messageThreadId,
    optional<std::int32_t> directMessagesTopicId,
    optional<std::int32_t> videoStartTimestamp,
    optional<bool> showCaptionAboveMedia, optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<MessageId>(sendRequest(
        _bot_api_baseurl, _httpClient, "copyMessage",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"from_chat_id", std::move(fromChatId)},
        std::pair{"message_id", messageId}, std::pair{"caption", caption},
        std::pair{"parse_mode", parseMode},
        std::pair{"caption_entities", captionEntities},
        std::pair{"disable_notification", disableNotification},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"protect_content", protectContent},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"video_start_timestamp", videoStartTimestamp},
        std::pair{"show_caption_above_media", showCaptionAboveMedia},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

std::vector<MessageId::Ptr> Api::copyMessages(
    ChatIdType chatId, ChatIdType fromChatId,
    const std::vector<std::int32_t>& messageIds,
    optional<std::int32_t> messageThreadId, optional<bool> disableNotification,
    optional<bool> protectContent, optional<bool> removeCaption,
    optional<std::int32_t> directMessagesTopicId) const {
    return parseArray<MessageId>(
        sendRequest(_bot_api_baseurl, _httpClient, "copyMessages",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"from_chat_id", std::move(fromChatId)},
                    std::pair{"message_ids", messageIds},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"protect_content", protectContent},
                    std::pair{"remove_caption", removeCaption},
                    std::pair{"direct_messages_topic_id",
                              directMessagesTopicId}));
}

Message::Ptr Api::sendPhoto(
    ChatIdType chatId, FileHandleType photo,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> showCaptionAboveMedia, optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendPhoto",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"photo", std::move(photo)}, std::pair{"caption", caption},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"parse_mode", parseMode},
        std::pair{"disable_notification", disableNotification},
        std::pair{"caption_entities", captionEntities},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"has_spoiler", hasSpoiler},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"show_caption_above_media", showCaptionAboveMedia},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendAudio(
    ChatIdType chatId, FileHandleType audio,
    const optional<std::string_view> caption, optional<std::int32_t> duration,
    const optional<std::string_view> performer,
    const optional<std::string_view> title, FileHandleType thumbnail,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendAudio",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"audio", std::move(audio)}, std::pair{"caption", caption},
        std::pair{"duration", duration}, std::pair{"performer", performer},
        std::pair{"title", title}, std::pair{"thumbnail", std::move(thumbnail)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"parse_mode", parseMode},
        std::pair{"disable_notification", disableNotification},
        std::pair{"caption_entities", captionEntities},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendDocument(
    ChatIdType chatId, FileHandleType document, FileHandleType thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<bool> disableContentTypeDetection,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "sendDocument",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"document", std::move(document)},
                    std::pair{"thumbnail", std::move(thumbnail)},
                    std::pair{"caption", caption},
                    std::pair{"reply_parameters", std::move(replyParameters)},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"parse_mode", parseMode},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"caption_entities", captionEntities},
                    std::pair{"disable_content_type_detection",
                              disableContentTypeDetection},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"protect_content", protectContent},
                    std::pair{"business_connection_id", businessConnectionId},
                    std::pair{"direct_messages_topic_id", directMessagesTopicId},
                    std::pair{"allow_paid_broadcast", allowPaidBroadcast},
                    std::pair{"message_effect_id", messageEffectId},
                    std::pair{"suggested_post_parameters",
                              std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendVideo(
    ChatIdType chatId, FileHandleType video, optional<bool> supportsStreaming,
    optional<std::int32_t> duration, optional<std::int32_t> width,
    optional<std::int32_t> height, FileHandleType thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId, FileHandleType cover,
    optional<std::int32_t> startTimestamp, optional<bool> showCaptionAboveMedia,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "sendVideo",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"video", std::move(video)},
                    std::pair{"supports_streaming", supportsStreaming},
                    std::pair{"duration", duration}, std::pair{"width", width},
                    std::pair{"height", height},
                    std::pair{"thumbnail", std::move(thumbnail)},
                    std::pair{"caption", caption},
                    std::pair{"reply_parameters", std::move(replyParameters)},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"parse_mode", parseMode},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"caption_entities", captionEntities},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"protect_content", protectContent},
                    std::pair{"has_spoiler", hasSpoiler},
                    std::pair{"business_connection_id", businessConnectionId},
                    std::pair{"direct_messages_topic_id", directMessagesTopicId},
                    std::pair{"cover", std::move(cover)},
                    std::pair{"start_timestamp", startTimestamp},
                    std::pair{"show_caption_above_media", showCaptionAboveMedia},
                    std::pair{"allow_paid_broadcast", allowPaidBroadcast},
                    std::pair{"message_effect_id", messageEffectId},
                    std::pair{"suggested_post_parameters",
                              std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendAnimation(
    ChatIdType chatId, FileHandleType animation,
    optional<std::int32_t> duration, optional<std::int32_t> width,
    optional<std::int32_t> height, FileHandleType thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> showCaptionAboveMedia, optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "sendAnimation",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"animation", std::move(animation)},
                    std::pair{"duration", duration}, std::pair{"width", width},
                    std::pair{"height", height},
                    std::pair{"thumbnail", std::move(thumbnail)},
                    std::pair{"caption", caption},
                    std::pair{"reply_parameters", std::move(replyParameters)},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"parse_mode", parseMode},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"caption_entities", captionEntities},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"protect_content", protectContent},
                    std::pair{"has_spoiler", hasSpoiler},
                    std::pair{"business_connection_id", businessConnectionId},
                    std::pair{"direct_messages_topic_id", directMessagesTopicId},
                    std::pair{"show_caption_above_media", showCaptionAboveMedia},
                    std::pair{"allow_paid_broadcast", allowPaidBroadcast},
                    std::pair{"message_effect_id", messageEffectId},
                    std::pair{"suggested_post_parameters",
                              std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendVoice(
    ChatIdType chatId, FileHandleType voice,
    const optional<std::string_view> caption, optional<std::int32_t> duration,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendVoice",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"voice", std::move(voice)}, std::pair{"caption", caption},
        std::pair{"duration", duration},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"parse_mode", parseMode},
        std::pair{"disable_notification", disableNotification},
        std::pair{"caption_entities", captionEntities},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendVideoNote(
    ChatIdType chatId, FileHandleType videoNote,
    ReplyParameters::Ptr replyParameters, optional<bool> disableNotification,
    optional<std::int32_t> duration, optional<std::int32_t> length,
    FileHandleType thumbnail, GenericReply::Ptr replyMarkup,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendVideoNote",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"video_note", std::move(videoNote)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"duration", duration}, std::pair{"length", length},
        std::pair{"thumbnail", std::move(thumbnail)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

std::vector<Message::Ptr> Api::sendMediaGroup(
    ChatIdType chatId, const std::vector<InputMedia::Ptr>& media,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId) const {
    return parseArray<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendMediaGroup",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"media", media},
        std::pair{"disable_notification", disableNotification},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId}));
}

Message::Ptr Api::sendLocation(
    ChatIdType chatId, float latitude, float longitude,
    bounded_optional<std::int32_t, 60, 86400> livePeriod,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<bool> disableNotification,
    bounded_optional<float, 0, 1500> horizontalAccuracy,
    optional<std::int32_t> heading,
    bounded_optional<std::int32_t, 1, 100000> proximityAlertRadius,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendLocation",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"latitude", latitude}, std::pair{"longitude", longitude},
        std::pair{"live_period", livePeriod},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"horizontal_accuracy", horizontalAccuracy},
        std::pair{"heading", heading},
        std::pair{"proximity_alert_radius", proximityAlertRadius},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

Message::Ptr Api::editMessageLiveLocation(
    float latitude, float longitude, ChatIdType chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    InlineKeyboardMarkup::Ptr replyMarkup,
    bounded_optional<float, 0, 1500> horizontalAccuracy,
    bounded_optional<std::int32_t, 1, 360> heading,
    bounded_optional<std::int32_t, 1, 100000> proximityAlertRadius,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> livePeriod) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageLiveLocation",
        std::pair{"latitude", latitude}, std::pair{"longitude", longitude},
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"message_id", messageId},
        std::pair{"inline_message_id", inlineMessageId},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"horizontal_accuracy", horizontalAccuracy},
        std::pair{"heading", heading},
        std::pair{"proximity_alert_radius", proximityAlertRadius},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"live_period", livePeriod}));
}

Message::Ptr Api::stopMessageLiveLocation(
    ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    InlineKeyboardMarkup::Ptr replyMarkup,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "stopMessageLiveLocation",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"inline_message_id", inlineMessageId},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendVenue(
    ChatIdType chatId, float latitude, float longitude,
    const std::string_view title, const std::string_view address,
    const optional<std::string_view> foursquareId,
    const optional<std::string_view> foursquareType,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    GenericReply::Ptr replyMarkup,
    const optional<std::string_view> googlePlaceId,
    const optional<std::string_view> googlePlaceType,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendVenue",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"latitude", latitude}, std::pair{"longitude", longitude},
        std::pair{"title", title}, std::pair{"address", address},
        std::pair{"foursquare_id", foursquareId},
        std::pair{"foursquare_type", foursquareType},
        std::pair{"disable_notification", disableNotification},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"google_place_id", googlePlaceId},
        std::pair{"google_place_type", googlePlaceType},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendContact(
    ChatIdType chatId, const std::string_view phoneNumber,
    const std::string_view firstName, const optional<std::string_view> lastName,
    const optional<std::string_view> vcard, optional<bool> disableNotification,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "sendContact",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"phone_number", phoneNumber},
                    std::pair{"first_name", firstName},
                    std::pair{"last_name", lastName}, std::pair{"vcard", vcard},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"reply_parameters", std::move(replyParameters)},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"protect_content", protectContent},
                    std::pair{"business_connection_id", businessConnectionId},
                    std::pair{"direct_messages_topic_id", directMessagesTopicId},
                    std::pair{"allow_paid_broadcast", allowPaidBroadcast},
                    std::pair{"message_effect_id", messageEffectId},
                    std::pair{"suggested_post_parameters",
                              std::move(suggestedPostParameters)}));
}

Message::Ptr Api::sendPoll(
    ChatIdType chatId, const std::string_view question,
    const std::vector<std::string>& options, optional<bool> disableNotification,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional_default<bool, true> isAnonymous,
    const optional_default<PollType, PollType::regular> type,
    optional_default<bool, false> allowsMultipleAnswers,
    optional<std::int32_t> correctOptionId,
    const optional<std::string_view> explanation,
    const optional<ParseMode> explanationParseMode,
    const std::vector<MessageEntity::Ptr>& explanationEntities,
    optional<std::int32_t> openPeriod,
    optional<std::chrono::system_clock::time_point> closeDate,
    optional<bool> isClosed, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    const optional<ParseMode> questionParseMode,
    const std::vector<MessageEntity::Ptr>& questionEntities,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    optional<bool> allowsRevoting, optional<bool> shuffleOptions,
    optional<bool> allowAddingOptions, optional<bool> hideResultsUntilCloses,
    optional<bool> membersOnly, const std::vector<std::string>& countryCodes,
    const std::vector<std::int32_t>& correctOptionIds,
    const optional<ParseMode> descriptionParseMode,
    const std::vector<MessageEntity::Ptr>& descriptionEntities) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendPoll",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"question", question}, std::pair{"options", options},
        std::pair{"disable_notification", disableNotification},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"is_anonymous", isAnonymous}, std::pair{"type", type},
        std::pair{"allows_multiple_answers", allowsMultipleAnswers},
        std::pair{"correct_option_id", correctOptionId},
        std::pair{"explanation", explanation},
        std::pair{"explanation_parse_mode", explanationParseMode},
        std::pair{"explanation_entities", explanationEntities},
        std::pair{"open_period", openPeriod},
        std::pair{"close_date", closeDate}, std::pair{"is_closed", isClosed},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"question_parse_mode", questionParseMode},
        std::pair{"question_entities", questionEntities},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"allows_revoting", allowsRevoting},
        std::pair{"shuffle_options", shuffleOptions},
        std::pair{"allow_adding_options", allowAddingOptions},
        std::pair{"hide_results_until_closes", hideResultsUntilCloses},
        std::pair{"members_only", membersOnly},
        std::pair{"country_codes", countryCodes},
        std::pair{"correct_option_ids", correctOptionIds},
        std::pair{"description_parse_mode", descriptionParseMode},
        std::pair{"description_entities", descriptionEntities}));
}

Message::Ptr Api::sendDice(
    ChatIdType chatId, optional<bool> disableNotification,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<std::string_view> emoji,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "sendDice",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"reply_parameters", std::move(replyParameters)},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"emoji", emoji},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"protect_content", protectContent},
                    std::pair{"business_connection_id", businessConnectionId},
                    std::pair{"direct_messages_topic_id", directMessagesTopicId},
                    std::pair{"allow_paid_broadcast", allowPaidBroadcast},
                    std::pair{"message_effect_id", messageEffectId},
                    std::pair{"suggested_post_parameters",
                              std::move(suggestedPostParameters)}));
}

bool Api::setMessageReaction(ChatIdType chatId,
                             optional<std::int32_t> messageId,
                             const std::vector<ReactionType::Ptr>& reaction,
                             optional<bool> isBig) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMessageReaction",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId},
                       std::pair{"reaction", reaction},
                       std::pair{"is_big", isBig})
        .get<bool>();
}

bool Api::sendChatAction(
    std::int64_t chatId, const ChatAction action,
    optional<std::int32_t> messageThreadId,
    const optional<std::string_view> businessConnectionId) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "sendChatAction",
               std::pair{"chat_id", chatId}, std::pair{"action", action},
               std::pair{"message_thread_id", messageThreadId},
               std::pair{"business_connection_id", businessConnectionId})
        .get<bool>();
}

UserProfilePhotos::Ptr Api::getUserProfilePhotos(
    std::int64_t userId, optional<std::int32_t> offset,
    bounded_optional_default<std::int32_t, 1, 100, 100> limit) const {
    return parse<UserProfilePhotos>(
        sendRequest(_bot_api_baseurl, _httpClient, "getUserProfilePhotos",
                    std::pair{"user_id", userId}, std::pair{"offset", offset},
                    std::pair{"limit", limit}));
}

File::Ptr Api::getFile(const std::string_view fileId) const {
    return parse<File>(sendRequest(_bot_api_baseurl, _httpClient, "getFile",
                                   std::pair{"file_id", fileId}));
}

bool Api::banChatMember(
    ChatIdType chatId, std::int64_t userId,
    optional<std::chrono::system_clock::time_point> untilDate,
    optional<bool> revokeMessages) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "banChatMember",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId},
                       std::pair{"until_date", untilDate},
                       std::pair{"revoke_messages", revokeMessages})
        .get<bool>();
}

bool Api::unbanChatMember(ChatIdType chatId, std::int64_t userId,
                          optional<bool> onlyIfBanned) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unbanChatMember",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId},
                       std::pair{"only_if_banned", onlyIfBanned})
        .get<bool>();
}

bool Api::restrictChatMember(
    ChatIdType chatId, std::int64_t userId, ChatPermissions::Ptr permissions,
    optional<std::chrono::system_clock::time_point> untilDate,
    optional<bool> useIndependentChatPermissions) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "restrictChatMember",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId},
                       std::pair{"permissions", std::move(permissions)},
                       std::pair{"until_date", untilDate},
                       std::pair{"use_independent_chat_permissions",
                                 useIndependentChatPermissions})
        .get<bool>();
}

bool Api::promoteChatMember(
    ChatIdType chatId, std::int64_t userId, optional<bool> canChangeInfo,
    optional<bool> canPostMessages, optional<bool> canEditMessages,
    optional<bool> canDeleteMessages, optional<bool> canInviteUsers,
    optional<bool> canPinMessages, optional<bool> canPromoteMembers,
    optional<bool> isAnonymous, optional<bool> canManageChat,
    optional<bool> canManageVideoChats, optional<bool> canRestrictMembers,
    optional<bool> canManageTopics, optional<bool> canPostStories,
    optional<bool> canEditStories, optional<bool> canDeleteStories,
    optional<bool> canManageDirectMessages, optional<bool> canManageTags) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "promoteChatMember",
               std::pair{"chat_id", std::move(chatId)},
               std::pair{"user_id", userId},
               std::pair{"can_change_info", canChangeInfo},
               std::pair{"can_post_messages", canPostMessages},
               std::pair{"can_edit_messages", canEditMessages},
               std::pair{"can_delete_messages", canDeleteMessages},
               std::pair{"can_invite_users", canInviteUsers},
               std::pair{"can_pin_messages", canPinMessages},
               std::pair{"can_promote_members", canPromoteMembers},
               std::pair{"is_anonymous", isAnonymous},
               std::pair{"can_manage_chat", canManageChat},
               std::pair{"can_manage_video_chats", canManageVideoChats},
               std::pair{"can_restrict_members", canRestrictMembers},
               std::pair{"can_manage_topics", canManageTopics},
               std::pair{"can_post_stories", canPostStories},
               std::pair{"can_edit_stories", canEditStories},
               std::pair{"can_delete_stories", canDeleteStories},
               std::pair{"can_manage_direct_messages", canManageDirectMessages},
               std::pair{"can_manage_tags", canManageTags})
        .get<bool>();
}

bool Api::setChatAdministratorCustomTitle(
    ChatIdType chatId, std::int64_t userId,
    const std::string_view customTitle) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setChatAdministratorCustomTitle",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId},
                       std::pair{"custom_title", customTitle})
        .get<bool>();
}

bool Api::banChatSenderChat(ChatIdType chatId,
                            std::int64_t senderChatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "banChatSenderChat",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"sender_chat_id", senderChatId})
        .get<bool>();
}

bool Api::unbanChatSenderChat(ChatIdType chatId,
                              std::int64_t senderChatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unbanChatSenderChat",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"sender_chat_id", senderChatId})
        .get<bool>();
}

bool Api::setChatPermissions(
    ChatIdType chatId, ChatPermissions::Ptr permissions,
    optional<bool> useIndependentChatPermissions) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatPermissions",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"permissions", std::move(permissions)},
                       std::pair{"use_independent_chat_permissions",
                                 useIndependentChatPermissions})
        .get<bool>();
}

std::string Api::exportChatInviteLink(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "exportChatInviteLink",
                       std::pair{"chat_id", std::move(chatId)})
        .get<std::string>();
}

ChatInviteLink::Ptr Api::createChatInviteLink(
    ChatIdType chatId,
    optional<std::chrono::system_clock::time_point> expireDate,
    optional<std::int32_t> memberLimit, const optional<std::string_view> name,
    optional<bool> createsJoinRequest) const {
    return parse<ChatInviteLink>(sendRequest(
        _bot_api_baseurl, _httpClient, "createChatInviteLink",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"expire_date", expireDate},
        std::pair{"member_limit", memberLimit}, std::pair{"name", name},
        std::pair{"creates_join_request", createsJoinRequest}));
}

ChatInviteLink::Ptr Api::editChatInviteLink(
    ChatIdType chatId, const std::string_view inviteLink,
    optional<std::chrono::system_clock::time_point> expireDate,
    optional<std::int32_t> memberLimit, const optional<std::string_view> name,
    optional<bool> createsJoinRequest) const {
    return parse<ChatInviteLink>(sendRequest(
        _bot_api_baseurl, _httpClient, "editChatInviteLink",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"invite_link", inviteLink},
        std::pair{"expire_date", expireDate},
        std::pair{"member_limit", memberLimit}, std::pair{"name", name},
        std::pair{"creates_join_request", createsJoinRequest}));
}

ChatInviteLink::Ptr Api::revokeChatInviteLink(
    ChatIdType chatId, const std::string_view inviteLink) const {
    return parse<ChatInviteLink>(
        sendRequest(_bot_api_baseurl, _httpClient, "revokeChatInviteLink",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"invite_link", inviteLink}));
}

bool Api::approveChatJoinRequest(ChatIdType chatId, std::int64_t userId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "approveChatJoinRequest",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId})
        .get<bool>();
}

bool Api::declineChatJoinRequest(ChatIdType chatId, std::int64_t userId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "declineChatJoinRequest",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId})
        .get<bool>();
}

bool Api::setChatPhoto(ChatIdType chatId, InputFile::Ptr photo) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatPhoto",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"photo", std::move(photo)})
        .get<bool>();
}

bool Api::deleteChatPhoto(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteChatPhoto",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::setChatTitle(ChatIdType chatId, const std::string_view title) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatTitle",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"title", title})
        .get<bool>();
}

bool Api::setChatDescription(ChatIdType chatId,
                             const std::string_view description) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatDescription",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"description", description})
        .get<bool>();
}

bool Api::pinChatMessage(
    ChatIdType chatId, std::int32_t messageId,
    optional<bool> disableNotification,
    const optional<std::string_view> businessConnectionId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "pinChatMessage",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId},
                       std::pair{"disable_notification", disableNotification},
                       std::pair{"business_connection_id", businessConnectionId})
        .get<bool>();
}

bool Api::unpinChatMessage(
    ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> businessConnectionId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unpinChatMessage",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId},
                       std::pair{"business_connection_id", businessConnectionId})
        .get<bool>();
}

bool Api::unpinAllChatMessages(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unpinAllChatMessages",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::leaveChat(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "leaveChat",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

Chat::Ptr Api::getChat(ChatIdType chatId) const {
    return parse<Chat>(sendRequest(_bot_api_baseurl, _httpClient, "getChat",
                                   std::pair{"chat_id", std::move(chatId)}));
}

std::vector<ChatMember::Ptr> Api::getChatAdministrators(
    ChatIdType chatId, optional<bool> returnBots) const {
    return parseArray<ChatMember>(
        sendRequest(_bot_api_baseurl, _httpClient, "getChatAdministrators",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"return_bots", returnBots}));
}

int32_t Api::getChatMemberCount(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "getChatMemberCount",
                       std::pair{"chat_id", std::move(chatId)})
        .get<int>();
}

ChatMember::Ptr Api::getChatMember(ChatIdType chatId,
                                   std::int64_t userId) const {
    return parse<ChatMember>(sendRequest(
        _bot_api_baseurl, _httpClient, "getChatMember",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"user_id", userId}));
}

bool Api::setChatStickerSet(ChatIdType chatId,
                            const std::string_view stickerSetName) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatStickerSet",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"sticker_set_name", stickerSetName})
        .get<bool>();
}

bool Api::deleteChatStickerSet(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteChatStickerSet",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

std::vector<Sticker::Ptr> Api::getForumTopicIconStickers() const {
    return parseArray<Sticker>(sendRequest(_bot_api_baseurl, _httpClient,
                                           "getForumTopicIconStickers"));
}

ForumTopic::Ptr Api::createForumTopic(
    ChatIdType chatId, const std::string_view name,
    optional<std::int32_t> iconColor,
    const optional<std::string_view> iconCustomEmojiId) const {
    return parse<ForumTopic>(
        sendRequest(_bot_api_baseurl, _httpClient, "createForumTopic",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"name", name}, std::pair{"icon_color", iconColor},
                    std::pair{"icon_custom_emoji_id", iconCustomEmojiId}));
}

bool Api::editForumTopic(
    ChatIdType chatId, std::int32_t messageThreadId,
    const optional<std::string_view> name,
    std::variant<std::int32_t, std::string> iconCustomEmojiId) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "editForumTopic",
               std::pair{"chat_id", std::move(chatId)},
               std::pair{"message_thread_id", messageThreadId},
               std::pair{"name", name},
               std::pair{"icon_custom_emoji_id", std::move(iconCustomEmojiId)})
        .get<bool>();
}

bool Api::closeForumTopic(ChatIdType chatId,
                          std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "closeForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .get<bool>();
}

bool Api::reopenForumTopic(ChatIdType chatId,
                           std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "reopenForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .get<bool>();
}

bool Api::deleteForumTopic(ChatIdType chatId,
                           std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .get<bool>();
}

bool Api::unpinAllForumTopicMessages(ChatIdType chatId,
                                     std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "unpinAllForumTopicMessages",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .get<bool>();
}

bool Api::editGeneralForumTopic(ChatIdType chatId, std::string name) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "editGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"name", std::move(name)})
        .get<bool>();
}

bool Api::closeGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "closeGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::reopenGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "reopenGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::hideGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "hideGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::unhideGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unhideGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::unpinAllGeneralForumTopicMessages(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "unpinAllGeneralForumTopicMessages",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::answerCallbackQuery(const std::string_view callbackQueryId,
                              const optional<std::string_view> text,
                              optional<bool> showAlert,
                              const optional<std::string_view> url,
                              optional<std::int32_t> cacheTime) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "answerCallbackQuery",
               std::pair{"callback_query_id", callbackQueryId},
               std::pair{"text", text}, std::pair{"show_alert", showAlert},
               std::pair{"url", url}, std::pair{"cache_time", cacheTime})
        .get<bool>();
}

UserChatBoosts::Ptr Api::getUserChatBoosts(ChatIdType chatId,
                                           std::int32_t userId) const {
    return parse<UserChatBoosts>(sendRequest(
        _bot_api_baseurl, _httpClient, "getUserChatBoosts",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"user_id", userId}));
}

BusinessConnection::Ptr Api::getBusinessConnection(
    const std::string_view businessConnectionId) const {
    return parse<BusinessConnection>(
        sendRequest(_bot_api_baseurl, _httpClient, "getBusinessConnection",
                    std::pair{"business_connection_id", businessConnectionId}));
}

bool Api::setMyCommands(const std::vector<BotCommand::Ptr>& commands,
                        BotCommandScope::Ptr scope,
                        const optional<LanguageCode> languageCode) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMyCommands",
                       std::pair{"commands", commands},
                       std::pair{"scope", std::move(scope)},
                       std::pair{"language_code", languageCode})
        .get<bool>();
}

bool Api::deleteMyCommands(BotCommandScope::Ptr scope,
                           const optional<LanguageCode> languageCode) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteMyCommands",
                       std::pair{"scope", std::move(scope)},
                       std::pair{"language_code", languageCode})
        .get<bool>();
}

std::vector<BotCommand::Ptr> Api::getMyCommands(
    BotCommandScope::Ptr scope,
    const optional<LanguageCode> languageCode) const {
    return parseArray<BotCommand>(
        sendRequest(_bot_api_baseurl, _httpClient, "getMyCommands",
                    std::pair{"scope", std::move(scope)},
                    std::pair{"language_code", languageCode}));
}

bool Api::setMyName(const optional<std::string_view> name,
                    const optional<LanguageCode> languageCode) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMyName",
                       std::pair{"name", name},
                       std::pair{"language_code", languageCode})
        .get<bool>();
}

BotName::Ptr Api::getMyName(const optional<LanguageCode> languageCode) const {
    return parse<BotName>(
        sendRequest(_bot_api_baseurl, _httpClient, "getMyName",
                    std::pair{"language_code", languageCode}));
}

bool Api::setMyDescription(const optional<std::string_view> description,
                           const optional<LanguageCode> languageCode) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMyDescription",
                       std::pair{"description", description},
                       std::pair{"language_code", languageCode})
        .get<bool>();
}

BotDescription::Ptr Api::getMyDescription(
    const optional<LanguageCode> languageCode) const {
    return parse<BotDescription>(
        sendRequest(_bot_api_baseurl, _httpClient, "getMyDescription",
                    std::pair{"language_code", languageCode}));
}

bool Api::setMyShortDescription(
    const optional<std::string_view> shortDescription,
    const optional<LanguageCode> languageCode) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMyShortDescription",
                       std::pair{"short_description", shortDescription},
                       std::pair{"language_code", languageCode})
        .get<bool>();
}

BotShortDescription::Ptr Api::getMyShortDescription(
    const optional<LanguageCode> languageCode) const {
    return parse<BotShortDescription>(
        sendRequest(_bot_api_baseurl, _httpClient, "getMyShortDescription",
                    std::pair{"language_code", languageCode}));
}

bool Api::setChatMenuButton(optional<std::int64_t> chatId,
                            MenuButton::Ptr menuButton) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatMenuButton",
                       std::pair{"chat_id", chatId},
                       std::pair{"menu_button", std::move(menuButton)})
        .get<bool>();
}

MenuButton::Ptr Api::getChatMenuButton(optional<std::int64_t> chatId) const {
    return parse<MenuButton>(sendRequest(_bot_api_baseurl, _httpClient,
                                         "getChatMenuButton",
                                         std::pair{"chat_id", chatId}));
}

bool Api::setMyDefaultAdministratorRights(ChatAdministratorRights::Ptr rights,
                                          optional<bool> forChannels) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setMyDefaultAdministratorRights",
                       std::pair{"rights", std::move(rights)},
                       std::pair{"for_channels", forChannels})
        .get<bool>();
}

ChatAdministratorRights::Ptr Api::getMyDefaultAdministratorRights(
    optional<bool> forChannels) const {
    return parse<ChatAdministratorRights>(sendRequest(
        _bot_api_baseurl, _httpClient, "getMyDefaultAdministratorRights",
        std::pair{"for_channels", forChannels}));
}

Message::Ptr Api::editMessageText(
    const std::string_view text, ChatIdType chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    const optional<ParseMode> parseMode,
    LinkPreviewOptions::Ptr linkPreviewOptions,
    InlineKeyboardMarkup::Ptr replyMarkup,
    const std::vector<MessageEntity::Ptr>& entities,
    const optional<std::string_view> businessConnectionId) const {
    const auto p = sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageText",
        std::pair{"text", text}, std::pair{"chat_id", std::move(chatId)},
        std::pair{"message_id", messageId},
        std::pair{"inline_message_id", inlineMessageId},
        std::pair{"parse_mode", parseMode},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"entities", entities},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"link_preview_options", std::move(linkPreviewOptions)});
    if (p.contains("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageCaption(
    ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> caption,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup, const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    const optional<std::string_view> businessConnectionId,
    optional<bool> showCaptionAboveMedia) const {
    const auto p = sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageCaption",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"message_id", messageId}, std::pair{"caption", caption},
        std::pair{"inline_message_id", inlineMessageId},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"parse_mode", parseMode},
        std::pair{"caption_entities", captionEntities},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"show_caption_above_media", showCaptionAboveMedia});
    if (p.contains("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageMedia(
    InputMedia::Ptr media, ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup,
    const optional<std::string_view> businessConnectionId) const {
    const auto& p =
        sendRequest(_bot_api_baseurl, _httpClient, "editMessageMedia",
                    std::pair{"media", std::move(media)},
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"inline_message_id", inlineMessageId},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"business_connection_id", businessConnectionId});
    if (p.contains("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageReplyMarkup(
    ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup,
    const optional<std::string_view> businessConnectionId) const {
    const auto& p =
        sendRequest(_bot_api_baseurl, _httpClient, "editMessageReplyMarkup",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"inline_message_id", inlineMessageId},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"business_connection_id", businessConnectionId});
    if (p.contains("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Poll::Ptr Api::stopPoll(
    ChatIdType chatId, std::int64_t messageId,
    InlineKeyboardMarkup::Ptr replyMarkup,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Poll>(
        sendRequest(_bot_api_baseurl, _httpClient, "stopPoll",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"business_connection_id", businessConnectionId}));
}

bool Api::deleteMessage(ChatIdType chatId, std::int32_t messageId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteMessage",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId})
        .get<bool>();
}

bool Api::deleteMessages(ChatIdType chatId,
                         const std::vector<std::int32_t>& messageIds) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteMessages",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_ids", messageIds})
        .get<bool>();
}

Message::Ptr Api::sendSticker(
    ChatIdType chatId, FileHandleType sticker,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<bool> disableNotification, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent, const optional<std::string_view> emoji,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendSticker",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"sticker", std::move(sticker)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent}, std::pair{"emoji", emoji},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)}));
}

StickerSet::Ptr Api::getStickerSet(const std::string_view name) const {
    return parse<StickerSet>(sendRequest(_bot_api_baseurl, _httpClient,
                                         "getStickerSet",
                                         std::pair{"name", name}));
}

std::vector<Sticker::Ptr> Api::getCustomEmojiStickers(
    const std::vector<std::string>& customEmojiIds) const {
    return parseArray<Sticker>(
        sendRequest(_bot_api_baseurl, _httpClient, "getCustomEmojiStickers",
                    std::pair{"custom_emoji_ids", customEmojiIds}));
}

File::Ptr Api::uploadStickerFile(std::int64_t userId, InputFile::Ptr sticker,
                                 const StickerFormat stickerFormat) const {
    return parse<File>(sendRequest(
        _bot_api_baseurl, _httpClient, "uploadStickerFile",
        std::pair{"user_id", userId}, std::pair{"sticker", std::move(sticker)},
        std::pair{"sticker_format", stickerFormat}));
}

bool Api::createNewStickerSet(
    std::int64_t userId, const std::string_view name,
    const std::string_view title,
    const std::vector<InputSticker::Ptr>& stickers,
    optional_default<Sticker::Type, Sticker::Type::Regular> stickerType,
    optional<bool> needsRepainting) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "createNewStickerSet",
                       std::pair{"user_id", userId}, std::pair{"name", name},
                       std::pair{"title", title},
                       std::pair{"stickers", stickers},
                       std::pair{"sticker_type", stickerType},
                       std::pair{"needs_repainting", needsRepainting})
        .get<bool>();
}

bool Api::addStickerToSet(std::int64_t userId, const std::string_view name,
                          InputSticker::Ptr sticker) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "addStickerToSet",
                       std::pair{"user_id", userId}, std::pair{"name", name},
                       std::pair{"sticker", std::move(sticker)})
        .get<bool>();
}

bool Api::setStickerPositionInSet(const std::string_view sticker,
                                  std::int32_t position) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerPositionInSet",
                       std::pair{"sticker", sticker},
                       std::pair{"position", position})
        .get<bool>();
}

bool Api::deleteStickerFromSet(const std::string_view sticker) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteStickerFromSet",
                       std::pair{"sticker", sticker})
        .get<bool>();
}

bool Api::replaceStickerInSet(std::int64_t userId, const std::string_view name,
                              const std::string_view oldSticker,
                              InputSticker::Ptr sticker) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "replaceStickerInSet",
                       std::pair{"user_id", userId}, std::pair{"name", name},
                       std::pair{"old_sticker", oldSticker},
                       std::pair{"sticker", std::move(sticker)})
        .get<bool>();
}

bool Api::setStickerEmojiList(const std::string_view sticker,
                              const std::vector<std::string>& emojiList) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerEmojiList",
                       std::pair{"sticker", sticker},
                       std::pair{"emoji_list", emojiList})
        .get<bool>();
}

bool Api::setStickerKeywords(const std::string_view sticker,
                             const std::vector<std::string>& keywords) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerKeywords",
                       std::pair{"sticker", sticker},
                       std::pair{"keywords", keywords})
        .get<bool>();
}

bool Api::setStickerMaskPosition(const std::string_view sticker,
                                 MaskPosition::Ptr maskPosition) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerMaskPosition",
                       std::pair{"sticker", sticker},
                       std::pair{"mask_position", std::move(maskPosition)})
        .get<bool>();
}

bool Api::setStickerSetTitle(const std::string_view name,
                             const std::string_view title) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerSetTitle",
                       std::pair{"name", name}, std::pair{"title", title})
        .get<bool>();
}

bool Api::setStickerSetThumbnail(const std::string_view name,
                                 std::int64_t userId,
                                 const StickerFormat format,
                                 FileHandleType thumbnail) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerSetThumbnail",
                       std::pair{"name", name}, std::pair{"user_id", userId},
                       std::pair{"format", format},
                       std::pair{"thumbnail", std::move(thumbnail)})
        .get<bool>();
}

bool Api::setCustomEmojiStickerSetThumbnail(
    const std::string_view name,
    const optional<std::string_view> customEmojiId) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setCustomEmojiStickerSetThumbnail",
                       std::pair{"name", name},
                       std::pair{"custom_emoji_id", customEmojiId})
        .get<bool>();
}

bool Api::deleteStickerSet(const std::string_view name) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteStickerSet",
                       std::pair{"name", name})
        .get<bool>();
}

bool Api::answerInlineQuery(const std::string_view inlineQueryId,
                            const std::vector<InlineQueryResult::Ptr>& results,
                            optional_default<std::int32_t, 300> cacheTime,
                            optional<bool> isPersonal,
                            const optional<std::string_view> nextOffset,
                            InlineQueryResultsButton::Ptr button) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "answerInlineQuery",
                       std::pair{"inline_query_id", inlineQueryId},
                       std::pair{"results", results},
                       std::pair{"cache_time", cacheTime},
                       std::pair{"is_personal", isPersonal},
                       std::pair{"next_offset", nextOffset},
                       std::pair{"button", std::move(button)})
        .get<bool>();
}

SentWebAppMessage::Ptr Api::answerWebAppQuery(
    const std::string_view webAppQueryId, InlineQueryResult::Ptr result) const {
    return parse<SentWebAppMessage>(
        sendRequest(_bot_api_baseurl, _httpClient, "answerWebAppQuery",
                    std::pair{"web_app_query_id", webAppQueryId},
                    std::pair{"result", std::move(result)}));
}

Message::Ptr Api::sendInvoice(
    ChatIdType chatId, const std::string_view title,
    const std::string_view description, const std::string_view payload,
    const std::string_view providerToken, const std::string_view currency,
    const std::vector<LabeledPrice::Ptr>& prices,
    const optional<std::string_view> providerData,
    const optional<std::string_view> photoUrl, optional<std::int32_t> photoSize,
    optional<std::int32_t> photoWidth, optional<std::int32_t> photoHeight,
    optional<bool> needName, optional<bool> needPhoneNumber,
    optional<bool> needEmail, optional<bool> needShippingAddress,
    optional<bool> sendPhoneNumberToProvider,
    optional<bool> sendEmailToProvider, optional<bool> isFlexible,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<bool> disableNotification, optional<std::int32_t> messageThreadId,
    optional<std::int32_t> maxTipAmount,
    const std::vector<std::int32_t>& suggestedTipAmounts,
    const optional<std::string_view> startParameter,
    optional<bool> protectContent, optional<std::int32_t> directMessagesTopicId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendInvoice",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"title", title},
        std::pair{"description", description}, std::pair{"payload", payload},
        std::pair{"provider_token", providerToken},
        std::pair{"currency", currency}, std::pair{"prices", prices},
        std::pair{"provider_data", providerData},
        std::pair{"photo_url", photoUrl}, std::pair{"photo_size", photoSize},
        std::pair{"photo_width", photoWidth},
        std::pair{"photo_height", photoHeight},
        std::pair{"need_name", needName},
        std::pair{"need_phone_number", needPhoneNumber},
        std::pair{"need_email", needEmail},
        std::pair{"need_shipping_address", needShippingAddress},
        std::pair{"send_phone_number_to_provider", sendPhoneNumberToProvider},
        std::pair{"send_email_to_provider", sendEmailToProvider},
        std::pair{"is_flexible", isFlexible},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"max_tip_amount", maxTipAmount},
        std::pair{"suggested_tip_amounts", suggestedTipAmounts},
        std::pair{"start_parameter", startParameter},
        std::pair{"protect_content", protectContent},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)},
        std::pair{"reply_parameters", std::move(replyParameters)}));
}

std::string Api::createInvoiceLink(
    const std::string_view title, const std::string_view description,
    const std::string_view payload, const std::string_view providerToken,
    const std::string_view currency,
    const std::vector<LabeledPrice::Ptr>& prices,
    optional<std::int32_t> maxTipAmount,
    const std::vector<std::int32_t>& suggestedTipAmounts,
    const optional<std::string_view> providerData,
    const optional<std::string_view> photoUrl, optional<std::int32_t> photoSize,
    optional<std::int32_t> photoWidth, optional<std::int32_t> photoHeight,
    optional<bool> needName, optional<bool> needPhoneNumber,
    optional<bool> needEmail, optional<bool> needShippingAddress,
    optional<bool> sendPhoneNumberToProvider,
    optional<bool> sendEmailToProvider, optional<bool> isFlexible,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> subscriptionPeriod) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "createInvoiceLink",
               std::pair{"title", title}, std::pair{"description", description},
               std::pair{"payload", payload},
               std::pair{"provider_token", providerToken},
               std::pair{"currency", currency}, std::pair{"prices", prices},
               std::pair{"max_tip_amount", maxTipAmount},
               std::pair{"suggested_tip_amounts", suggestedTipAmounts},
               std::pair{"provider_data", providerData},
               std::pair{"photo_url", photoUrl},
               std::pair{"photo_size", photoSize},
               std::pair{"photo_width", photoWidth},
               std::pair{"photo_height", photoHeight},
               std::pair{"need_name", needName},
               std::pair{"need_phone_number", needPhoneNumber},
               std::pair{"need_email", needEmail},
               std::pair{"need_shipping_address", needShippingAddress},
               std::pair{"send_phone_number_to_provider",
                         sendPhoneNumberToProvider},
               std::pair{"send_email_to_provider", sendEmailToProvider},
               std::pair{"is_flexible", isFlexible},
               std::pair{"business_connection_id", businessConnectionId},
               std::pair{"subscription_period", subscriptionPeriod})
        .get<std::string>();
}

bool Api::answerShippingQuery(
    const std::string_view shippingQueryId, bool ok,
    const std::vector<ShippingOption::Ptr>& shippingOptions,
    const optional<std::string_view> errorMessage) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "answerShippingQuery",
                       std::pair{"shipping_query_id", shippingQueryId},
                       std::pair{"ok", ok},
                       std::pair{"shipping_options", shippingOptions},
                       std::pair{"error_message", errorMessage})
        .get<bool>();
}

bool Api::answerPreCheckoutQuery(
    const std::string_view preCheckoutQueryId, bool ok,
    const optional<std::string_view> errorMessage) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "answerPreCheckoutQuery",
                       std::pair{"pre_checkout_query_id", preCheckoutQueryId},
                       std::pair{"ok", ok},
                       std::pair{"error_message", errorMessage})
        .get<bool>();
}

bool Api::setPassportDataErrors(
    std::int64_t userId,
    const std::vector<PassportElementError::Ptr>& errors) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setPassportDataErrors",
                       std::pair{"user_id", userId},
                       std::pair{"errors", errors})
        .get<bool>();
}

Message::Ptr Api::sendGame(
    std::int64_t chatId, const std::string_view gameShortName,
    ReplyParameters::Ptr replyParameters, InlineKeyboardMarkup::Ptr replyMarkup,
    optional<bool> disableNotification, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendGame", std::pair{"chat_id", chatId},
        std::pair{"game_short_name", gameShortName},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId}));
}

Message::Ptr Api::setGameScore(
    std::int64_t userId, std::int32_t score, optional<bool> force,
    optional<bool> disableEditMessage, optional<std::int64_t> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "setGameScore",
        std::pair{"user_id", userId}, std::pair{"score", score},
        std::pair{"force", force},
        std::pair{"disable_edit_message", disableEditMessage},
        std::pair{"chat_id", chatId}, std::pair{"message_id", messageId},
        std::pair{"inline_message_id", inlineMessageId}));
}

std::vector<GameHighScore::Ptr> Api::getGameHighScores(
    optional<std::int64_t> userId, optional<std::int64_t> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId) const {
    return parseArray<GameHighScore>(
        sendRequest(_bot_api_baseurl, _httpClient, "getGameHighScores",
                    std::pair{"user_id", userId}, std::pair{"chat_id", chatId},
                    std::pair{"message_id", messageId},
                    std::pair{"inline_message_id", inlineMessageId}));
}

std::string Api::downloadFile(const std::string_view filePath,
                              const HttpReqArg::Vec& args,
                              LocalFileMapper localFilePathMapper) const {
    std::string url(_url);

    static bool is_local = [&filePath] {
        std::filesystem::path dir = filePath;
        // Surely if the path is absolute, it is a local file
        return dir.is_absolute();
    }();

    if (is_local && !localFilePathMapper) {
        std::ifstream fileStream(std::string(filePath),
                                 std::ios::in | std::ios::binary);
        if (!fileStream) {
            throw TgException("Could not open file: " + std::string(filePath),
                              TgException::ErrorCode::Internal);
        }
        std::string fileContent((std::istreambuf_iterator<char>(fileStream)),
                                std::istreambuf_iterator<char>());
        return fileContent;
    } else if (is_local) {
        url = localFilePathMapper(filePath);
    } else {
        url += "/file/bot";
        url += _token;
        url += "/";
        url += filePath;
    }

    return _httpClient->makeRequest(url, args);
}

bool Api::blockedByUser(std::int64_t chatId) const {
    bool isBotBlocked = false;

    try {
        sendChatAction(chatId, ChatAction::typing);

    } catch (std::exception& e) {
        std::string error = e.what();

        if (error.find("Forbidden: bot was blocked by the user") ==
            std::string::npos) {
            isBotBlocked = true;
        }
    }

    return isBotBlocked;
}

Message::Ptr Api::sendPaidMedia(
    ChatIdType chatId, std::int32_t starCount,
    const std::vector<InputPaidMedia::Ptr>& media,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> messageThreadId,
    optional<std::int32_t> directMessagesTopicId,
    const optional<std::string_view> payload,
    const optional<std::string_view> caption, const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<bool> showCaptionAboveMedia, optional<bool> disableNotification,
    optional<bool> protectContent, optional<bool> allowPaidBroadcast,
    SuggestedPostParameters::Ptr suggestedPostParameters,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendPaidMedia",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"star_count", starCount}, std::pair{"media", media},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"payload", payload}, std::pair{"caption", caption},
        std::pair{"parse_mode", parseMode},
        std::pair{"caption_entities", captionEntities},
        std::pair{"show_caption_above_media", showCaptionAboveMedia},
        std::pair{"disable_notification", disableNotification},
        std::pair{"protect_content", protectContent},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"suggested_post_parameters", std::move(suggestedPostParameters)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)}));
}

Message::Ptr Api::sendChecklist(
    const std::string_view businessConnectionId, std::int64_t chatId,
    InputChecklist::Ptr checklist, optional<bool> disableNotification,
    optional<bool> protectContent, const optional<std::string_view> messageEffectId,
    ReplyParameters::Ptr replyParameters,
    InlineKeyboardMarkup::Ptr replyMarkup) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendChecklist",
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"chat_id", chatId},
        std::pair{"checklist", std::move(checklist)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"protect_content", protectContent},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)}));
}

bool Api::sendMessageDraft(std::int64_t chatId, std::int32_t draftId,
                           const std::string_view text,
                           optional<std::int32_t> messageThreadId,
                           const optional<ParseMode> parseMode,
                           const std::vector<MessageEntity::Ptr>& entities) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "sendMessageDraft",
                       std::pair{"chat_id", chatId},
                       std::pair{"draft_id", draftId}, std::pair{"text", text},
                       std::pair{"message_thread_id", messageThreadId},
                       std::pair{"parse_mode", parseMode},
                       std::pair{"entities", entities})
        .get<bool>();
}

UserProfileAudios::Ptr Api::getUserProfileAudios(
    std::int64_t userId, optional<std::int32_t> offset,
    optional<std::int32_t> limit) const {
    return parse<UserProfileAudios>(
        sendRequest(_bot_api_baseurl, _httpClient, "getUserProfileAudios",
                    std::pair{"user_id", userId}, std::pair{"offset", offset},
                    std::pair{"limit", limit}));
}

bool Api::setUserEmojiStatus(
    std::int64_t userId,
    const optional<std::string_view> emojiStatusCustomEmojiId,
    optional<std::int32_t> emojiStatusExpirationDate) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "setUserEmojiStatus",
               std::pair{"user_id", userId},
               std::pair{"emoji_status_custom_emoji_id", emojiStatusCustomEmojiId},
               std::pair{"emoji_status_expiration_date",
                         emojiStatusExpirationDate})
        .get<bool>();
}

bool Api::setChatMemberTag(ChatIdType chatId, std::int64_t userId,
                           const optional<std::string_view> tag) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatMemberTag",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId}, std::pair{"tag", tag})
        .get<bool>();
}

ChatInviteLink::Ptr Api::createChatSubscriptionInviteLink(
    ChatIdType chatId, std::int32_t subscriptionPeriod,
    std::int32_t subscriptionPrice, const optional<std::string_view> name) const {
    return parse<ChatInviteLink>(sendRequest(
        _bot_api_baseurl, _httpClient, "createChatSubscriptionInviteLink",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"subscription_period", subscriptionPeriod},
        std::pair{"subscription_price", subscriptionPrice},
        std::pair{"name", name}));
}

ChatInviteLink::Ptr Api::editChatSubscriptionInviteLink(
    ChatIdType chatId, const std::string_view inviteLink,
    const optional<std::string_view> name) const {
    return parse<ChatInviteLink>(sendRequest(
        _bot_api_baseurl, _httpClient, "editChatSubscriptionInviteLink",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"invite_link", inviteLink}, std::pair{"name", name}));
}

bool Api::setMyProfilePhoto(InputProfilePhoto::Ptr photo) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMyProfilePhoto",
                       std::pair{"photo", std::move(photo)})
        .get<bool>();
}

bool Api::removeMyProfilePhoto() const {
    return sendRequest(_bot_api_baseurl, _httpClient, "removeMyProfilePhoto")
        .get<bool>();
}

Gifts::Ptr Api::getAvailableGifts() const {
    return parse<Gifts>(
        sendRequest(_bot_api_baseurl, _httpClient, "getAvailableGifts"));
}

bool Api::sendGift(const std::string_view giftId, optional<std::int64_t> userId,
                   optional<ChatIdType> chatId, optional<bool> payForUpgrade,
                   const optional<std::string_view> text,
                   const optional<ParseMode> textParseMode,
                   const std::vector<MessageEntity::Ptr>& textEntities) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "sendGift",
                       std::pair{"gift_id", giftId},
                       std::pair{"user_id", userId},
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"pay_for_upgrade", payForUpgrade},
                       std::pair{"text", text},
                       std::pair{"text_parse_mode", textParseMode},
                       std::pair{"text_entities", textEntities})
        .get<bool>();
}

bool Api::giftPremiumSubscription(
    std::int64_t userId, std::int32_t monthCount, std::int32_t starCount,
    const optional<std::string_view> text,
    const optional<ParseMode> textParseMode,
    const std::vector<MessageEntity::Ptr>& textEntities) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "giftPremiumSubscription",
                       std::pair{"user_id", userId},
                       std::pair{"month_count", monthCount},
                       std::pair{"star_count", starCount},
                       std::pair{"text", text},
                       std::pair{"text_parse_mode", textParseMode},
                       std::pair{"text_entities", textEntities})
        .get<bool>();
}

bool Api::verifyUser(std::int64_t userId,
                     const optional<std::string_view> customDescription) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "verifyUser",
                       std::pair{"user_id", userId},
                       std::pair{"custom_description", customDescription})
        .get<bool>();
}

bool Api::verifyChat(ChatIdType chatId,
                     const optional<std::string_view> customDescription) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "verifyChat",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"custom_description", customDescription})
        .get<bool>();
}

bool Api::removeUserVerification(std::int64_t userId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "removeUserVerification",
                       std::pair{"user_id", userId})
        .get<bool>();
}

bool Api::removeChatVerification(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "removeChatVerification",
                       std::pair{"chat_id", std::move(chatId)})
        .get<bool>();
}

bool Api::readBusinessMessage(const std::string_view businessConnectionId,
                              std::int64_t chatId, std::int32_t messageId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "readBusinessMessage",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"chat_id", chatId},
                       std::pair{"message_id", messageId})
        .get<bool>();
}

bool Api::deleteBusinessMessages(
    const std::string_view businessConnectionId,
    const std::vector<std::int32_t>& messageIds) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteBusinessMessages",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"message_ids", messageIds})
        .get<bool>();
}

bool Api::setBusinessAccountName(
    const std::string_view businessConnectionId,
    const std::string_view firstName,
    const optional<std::string_view> lastName) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setBusinessAccountName",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"first_name", firstName},
                       std::pair{"last_name", lastName})
        .get<bool>();
}

bool Api::setBusinessAccountUsername(
    const std::string_view businessConnectionId,
    const optional<std::string_view> username) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setBusinessAccountUsername",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"username", username})
        .get<bool>();
}

bool Api::setBusinessAccountBio(
    const std::string_view businessConnectionId,
    const optional<std::string_view> bio) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setBusinessAccountBio",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"bio", bio})
        .get<bool>();
}

bool Api::setBusinessAccountProfilePhoto(
    const std::string_view businessConnectionId, InputProfilePhoto::Ptr photo,
    optional<bool> isPublic) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setBusinessAccountProfilePhoto",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"photo", std::move(photo)},
                       std::pair{"is_public", isPublic})
        .get<bool>();
}

bool Api::removeBusinessAccountProfilePhoto(
    const std::string_view businessConnectionId, optional<bool> isPublic) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "removeBusinessAccountProfilePhoto",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"is_public", isPublic})
        .get<bool>();
}

bool Api::setBusinessAccountGiftSettings(
    const std::string_view businessConnectionId, bool showGiftButton,
    AcceptedGiftTypes::Ptr acceptedGiftTypes) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setBusinessAccountGiftSettings",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"show_gift_button", showGiftButton},
                       std::pair{"accepted_gift_types",
                                 std::move(acceptedGiftTypes)})
        .get<bool>();
}

StarAmount::Ptr Api::getBusinessAccountStarBalance(
    const std::string_view businessConnectionId) const {
    return parse<StarAmount>(sendRequest(
        _bot_api_baseurl, _httpClient, "getBusinessAccountStarBalance",
        std::pair{"business_connection_id", businessConnectionId}));
}

bool Api::transferBusinessAccountStars(
    const std::string_view businessConnectionId, std::int32_t starCount) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "transferBusinessAccountStars",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"star_count", starCount})
        .get<bool>();
}

OwnedGifts::Ptr Api::getBusinessAccountGifts(
    const std::string_view businessConnectionId, optional<bool> excludeUnsaved,
    optional<bool> excludeSaved, optional<bool> excludeUnlimited,
    optional<bool> excludeLimitedUpgradable,
    optional<bool> excludeLimitedNonUpgradable, optional<bool> excludeUnique,
    optional<bool> excludeFromBlockchain, optional<bool> sortByPrice,
    const optional<std::string_view> offset, optional<std::int32_t> limit) const {
    return parse<OwnedGifts>(sendRequest(
        _bot_api_baseurl, _httpClient, "getBusinessAccountGifts",
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"exclude_unsaved", excludeUnsaved},
        std::pair{"exclude_saved", excludeSaved},
        std::pair{"exclude_unlimited", excludeUnlimited},
        std::pair{"exclude_limited_upgradable", excludeLimitedUpgradable},
        std::pair{"exclude_limited_non_upgradable",
                  excludeLimitedNonUpgradable},
        std::pair{"exclude_unique", excludeUnique},
        std::pair{"exclude_from_blockchain", excludeFromBlockchain},
        std::pair{"sort_by_price", sortByPrice}, std::pair{"offset", offset},
        std::pair{"limit", limit}));
}

OwnedGifts::Ptr Api::getUserGifts(
    std::int64_t userId, optional<bool> excludeUnlimited,
    optional<bool> excludeLimitedUpgradable,
    optional<bool> excludeLimitedNonUpgradable,
    optional<bool> excludeFromBlockchain, optional<bool> excludeUnique,
    optional<bool> sortByPrice, const optional<std::string_view> offset,
    optional<std::int32_t> limit) const {
    return parse<OwnedGifts>(sendRequest(
        _bot_api_baseurl, _httpClient, "getUserGifts",
        std::pair{"user_id", userId},
        std::pair{"exclude_unlimited", excludeUnlimited},
        std::pair{"exclude_limited_upgradable", excludeLimitedUpgradable},
        std::pair{"exclude_limited_non_upgradable",
                  excludeLimitedNonUpgradable},
        std::pair{"exclude_from_blockchain", excludeFromBlockchain},
        std::pair{"exclude_unique", excludeUnique},
        std::pair{"sort_by_price", sortByPrice}, std::pair{"offset", offset},
        std::pair{"limit", limit}));
}

OwnedGifts::Ptr Api::getChatGifts(
    ChatIdType chatId, optional<bool> excludeUnsaved, optional<bool> excludeSaved,
    optional<bool> excludeUnlimited, optional<bool> excludeLimitedUpgradable,
    optional<bool> excludeLimitedNonUpgradable,
    optional<bool> excludeFromBlockchain, optional<bool> excludeUnique,
    optional<bool> sortByPrice, const optional<std::string_view> offset,
    optional<std::int32_t> limit) const {
    return parse<OwnedGifts>(sendRequest(
        _bot_api_baseurl, _httpClient, "getChatGifts",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"exclude_unsaved", excludeUnsaved},
        std::pair{"exclude_saved", excludeSaved},
        std::pair{"exclude_unlimited", excludeUnlimited},
        std::pair{"exclude_limited_upgradable", excludeLimitedUpgradable},
        std::pair{"exclude_limited_non_upgradable",
                  excludeLimitedNonUpgradable},
        std::pair{"exclude_from_blockchain", excludeFromBlockchain},
        std::pair{"exclude_unique", excludeUnique},
        std::pair{"sort_by_price", sortByPrice}, std::pair{"offset", offset},
        std::pair{"limit", limit}));
}

bool Api::convertGiftToStars(const std::string_view businessConnectionId,
                             const std::string_view ownedGiftId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "convertGiftToStars",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"owned_gift_id", ownedGiftId})
        .get<bool>();
}

bool Api::upgradeGift(const std::string_view businessConnectionId,
                      const std::string_view ownedGiftId,
                      optional<bool> keepOriginalDetails,
                      optional<std::int32_t> starCount) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "upgradeGift",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"owned_gift_id", ownedGiftId},
                       std::pair{"keep_original_details", keepOriginalDetails},
                       std::pair{"star_count", starCount})
        .get<bool>();
}

bool Api::transferGift(const std::string_view businessConnectionId,
                       const std::string_view ownedGiftId,
                       std::int64_t newOwnerChatId,
                       optional<std::int32_t> starCount) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "transferGift",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"owned_gift_id", ownedGiftId},
                       std::pair{"new_owner_chat_id", newOwnerChatId},
                       std::pair{"star_count", starCount})
        .get<bool>();
}

Story::Ptr Api::postStory(
    const std::string_view businessConnectionId, InputStoryContent::Ptr content,
    std::int32_t activePeriod, const optional<std::string_view> caption,
    const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    const std::vector<StoryArea::Ptr>& areas, optional<bool> postToChatPage,
    optional<bool> protectContent) const {
    return parse<Story>(sendRequest(
        _bot_api_baseurl, _httpClient, "postStory",
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"content", std::move(content)},
        std::pair{"active_period", activePeriod}, std::pair{"caption", caption},
        std::pair{"parse_mode", parseMode},
        std::pair{"caption_entities", captionEntities},
        std::pair{"areas", areas},
        std::pair{"post_to_chat_page", postToChatPage},
        std::pair{"protect_content", protectContent}));
}

Story::Ptr Api::repostStory(const std::string_view businessConnectionId,
                            std::int64_t fromChatId, std::int32_t fromStoryId,
                            std::int32_t activePeriod,
                            optional<bool> postToChatPage,
                            optional<bool> protectContent) const {
    return parse<Story>(sendRequest(
        _bot_api_baseurl, _httpClient, "repostStory",
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"from_chat_id", fromChatId},
        std::pair{"from_story_id", fromStoryId},
        std::pair{"active_period", activePeriod},
        std::pair{"post_to_chat_page", postToChatPage},
        std::pair{"protect_content", protectContent}));
}

Story::Ptr Api::editStory(
    const std::string_view businessConnectionId, std::int32_t storyId,
    InputStoryContent::Ptr content, const optional<std::string_view> caption,
    const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    const std::vector<StoryArea::Ptr>& areas) const {
    return parse<Story>(sendRequest(
        _bot_api_baseurl, _httpClient, "editStory",
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"story_id", storyId},
        std::pair{"content", std::move(content)}, std::pair{"caption", caption},
        std::pair{"parse_mode", parseMode},
        std::pair{"caption_entities", captionEntities},
        std::pair{"areas", areas}));
}

bool Api::deleteStory(const std::string_view businessConnectionId,
                      std::int32_t storyId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteStory",
                       std::pair{"business_connection_id", businessConnectionId},
                       std::pair{"story_id", storyId})
        .get<bool>();
}

Message::Ptr Api::editMessageChecklist(
    const std::string_view businessConnectionId, std::int64_t chatId,
    std::int32_t messageId, InputChecklist::Ptr checklist,
    InlineKeyboardMarkup::Ptr replyMarkup) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageChecklist",
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"chat_id", chatId}, std::pair{"message_id", messageId},
        std::pair{"checklist", std::move(checklist)},
        std::pair{"reply_markup", std::move(replyMarkup)}));
}

bool Api::approveSuggestedPost(std::int64_t chatId, std::int32_t messageId,
                               optional<std::int32_t> sendDate) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "approveSuggestedPost",
                       std::pair{"chat_id", chatId},
                       std::pair{"message_id", messageId},
                       std::pair{"send_date", sendDate})
        .get<bool>();
}

bool Api::declineSuggestedPost(std::int64_t chatId, std::int32_t messageId,
                               const optional<std::string_view> comment) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "declineSuggestedPost",
                       std::pair{"chat_id", chatId},
                       std::pair{"message_id", messageId},
                       std::pair{"comment", comment})
        .get<bool>();
}

PreparedInlineMessage::Ptr Api::savePreparedInlineMessage(
    std::int64_t userId, InlineQueryResult::Ptr result,
    optional<bool> allowUserChats, optional<bool> allowBotChats,
    optional<bool> allowGroupChats, optional<bool> allowChannelChats) const {
    return parse<PreparedInlineMessage>(sendRequest(
        _bot_api_baseurl, _httpClient, "savePreparedInlineMessage",
        std::pair{"user_id", userId}, std::pair{"result", std::move(result)},
        std::pair{"allow_user_chats", allowUserChats},
        std::pair{"allow_bot_chats", allowBotChats},
        std::pair{"allow_group_chats", allowGroupChats},
        std::pair{"allow_channel_chats", allowChannelChats}));
}

StarAmount::Ptr Api::getMyStarBalance() const {
    return parse<StarAmount>(
        sendRequest(_bot_api_baseurl, _httpClient, "getMyStarBalance"));
}

StarTransactions::Ptr Api::getStarTransactions(
    optional<std::int32_t> offset, optional<std::int32_t> limit) const {
    return parse<StarTransactions>(
        sendRequest(_bot_api_baseurl, _httpClient, "getStarTransactions",
                    std::pair{"offset", offset}, std::pair{"limit", limit}));
}

bool Api::refundStarPayment(
    std::int64_t userId,
    const std::string_view telegramPaymentChargeId) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "refundStarPayment",
               std::pair{"user_id", userId},
               std::pair{"telegram_payment_charge_id", telegramPaymentChargeId})
        .get<bool>();
}

bool Api::editUserStarSubscription(
    std::int64_t userId, const std::string_view telegramPaymentChargeId,
    bool isCanceled) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "editUserStarSubscription",
               std::pair{"user_id", userId},
               std::pair{"telegram_payment_charge_id", telegramPaymentChargeId},
               std::pair{"is_canceled", isCanceled})
        .get<bool>();
}

Message::Ptr Api::sendRichMessage(
    ChatIdType chatId, InputRichMessage::Ptr richMessage,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> messageThreadId,
    optional<std::int32_t> directMessagesTopicId,
    optional<bool> disableNotification, optional<bool> protectContent,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendRichMessage",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"rich_message", std::move(richMessage)},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId},
        std::pair{"disable_notification", disableNotification},
        std::pair{"protect_content", protectContent},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters",
                  std::move(suggestedPostParameters)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)}));
}

bool Api::sendRichMessageDraft(std::int64_t chatId, std::int32_t draftId,
                               InputRichMessage::Ptr richMessage,
                               optional<std::int32_t> messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "sendRichMessageDraft",
                       std::pair{"chat_id", chatId},
                       std::pair{"draft_id", draftId},
                       std::pair{"rich_message", std::move(richMessage)},
                       std::pair{"message_thread_id", messageThreadId})
        .get<bool>();
}

Message::Ptr Api::sendLivePhoto(
    ChatIdType chatId, FileHandleType livePhoto, FileHandleType photo,
    const optional<std::string_view> caption, const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr>& captionEntities,
    optional<bool> showCaptionAboveMedia, optional<bool> hasSpoiler,
    optional<bool> disableNotification, optional<bool> protectContent,
    optional<bool> allowPaidBroadcast,
    const optional<std::string_view> messageEffectId,
    SuggestedPostParameters::Ptr suggestedPostParameters,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<std::string_view> businessConnectionId,
    optional<std::int32_t> messageThreadId,
    optional<std::int32_t> directMessagesTopicId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendLivePhoto",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"live_photo", std::move(livePhoto)},
        std::pair{"photo", std::move(photo)}, std::pair{"caption", caption},
        std::pair{"parse_mode", parseMode},
        std::pair{"caption_entities", captionEntities},
        std::pair{"show_caption_above_media", showCaptionAboveMedia},
        std::pair{"has_spoiler", hasSpoiler},
        std::pair{"disable_notification", disableNotification},
        std::pair{"protect_content", protectContent},
        std::pair{"allow_paid_broadcast", allowPaidBroadcast},
        std::pair{"message_effect_id", messageEffectId},
        std::pair{"suggested_post_parameters", std::move(suggestedPostParameters)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"business_connection_id", businessConnectionId},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"direct_messages_topic_id", directMessagesTopicId}));
}

bool Api::answerChatJoinRequestQuery(
    const std::string_view chatJoinRequestQueryId,
    const std::string_view result) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "answerChatJoinRequestQuery",
               std::pair{"chat_join_request_query_id", chatJoinRequestQueryId},
               std::pair{"result", result})
        .get<bool>();
}

bool Api::sendChatJoinRequestWebApp(
    const std::string_view chatJoinRequestQueryId,
    const std::string_view webAppUrl) const {
    return sendRequest(
               _bot_api_baseurl, _httpClient, "sendChatJoinRequestWebApp",
               std::pair{"chat_join_request_query_id", chatJoinRequestQueryId},
               std::pair{"web_app_url", webAppUrl})
        .get<bool>();
}

std::vector<Message::Ptr> Api::getUserPersonalChatMessages(
    std::int64_t userId, std::int32_t limit) const {
    return parseArray<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "getUserPersonalChatMessages",
                    std::pair{"user_id", userId}, std::pair{"limit", limit}));
}

SentGuestMessage::Ptr Api::answerGuestQuery(
    const std::string_view guestQueryId, InlineQueryResult::Ptr result) const {
    return parse<SentGuestMessage>(
        sendRequest(_bot_api_baseurl, _httpClient, "answerGuestQuery",
                    std::pair{"guest_query_id", guestQueryId},
                    std::pair{"result", std::move(result)}));
}

std::string Api::getManagedBotToken(std::int64_t userId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "getManagedBotToken",
                       std::pair{"user_id", userId})
        .get<std::string>();
}

std::string Api::replaceManagedBotToken(std::int64_t userId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "replaceManagedBotToken",
                       std::pair{"user_id", userId})
        .get<std::string>();
}

BotAccessSettings::Ptr Api::getManagedBotAccessSettings(
    std::int64_t userId) const {
    return parse<BotAccessSettings>(
        sendRequest(_bot_api_baseurl, _httpClient, "getManagedBotAccessSettings",
                    std::pair{"user_id", userId}));
}

bool Api::setManagedBotAccessSettings(
    std::int64_t userId, bool isAccessRestricted,
    const std::vector<std::int64_t>& addedUserIds) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setManagedBotAccessSettings",
                       std::pair{"user_id", userId},
                       std::pair{"is_access_restricted", isAccessRestricted},
                       std::pair{"added_user_ids", addedUserIds})
        .get<bool>();
}

PreparedKeyboardButton::Ptr Api::savePreparedKeyboardButton(
    std::int64_t userId, KeyboardButton::Ptr button) const {
    return parse<PreparedKeyboardButton>(
        sendRequest(_bot_api_baseurl, _httpClient, "savePreparedKeyboardButton",
                    std::pair{"user_id", userId},
                    std::pair{"button", std::move(button)}));
}

bool Api::deleteMessageReaction(ChatIdType chatId, std::int32_t messageId,
                                optional<std::int64_t> userId,
                                optional<std::int64_t> actorChatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteMessageReaction",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId},
                       std::pair{"user_id", userId},
                       std::pair{"actor_chat_id", actorChatId})
        .get<bool>();
}

bool Api::deleteAllMessageReactions(ChatIdType chatId,
                                    optional<std::int64_t> userId,
                                    optional<std::int64_t> actorChatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteAllMessageReactions",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId},
                       std::pair{"actor_chat_id", actorChatId})
        .get<bool>();
}

}  // namespace TgBot
