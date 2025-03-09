#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <tgbot/Api.h>
#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>
#include <tgbot/net/HttpReqArg.h>
#include <tgbot/tools/StringTools.h>

#include <chrono>
#include <cstdint>
#include <iostream>
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
auto putArg(std::string name, const T &data) {
    if constexpr (std::is_same_v<T, std::string_view> ||
                  detail::is_primitive_v<T>) {
        return std::make_unique<TgBot::HttpReqArg>(std::move(name), data);
    } else if constexpr (std::is_same_v<
                             T, std::chrono::system_clock::time_point>) {
        return std::make_unique<TgBot::HttpReqArg>(
            std::move(name), std::chrono::system_clock::to_time_t(data));
    } else if constexpr (std::is_same_v<T, TgBot::InputFile::Ptr>) {
        return std::make_unique<TgBot::HttpReqArgFile>(std::move(name), data);
    } else {
        return std::make_unique<TgBot::HttpReqArg>(std::move(name),
                                                   TgBot::putJSON(data));
    }
}

using TgBot::TgException;

constexpr bool kSendRequestDebug = false;

template <typename... Args>
Json::Value sendRequest(const std::string_view _bot_url,
                        TgBot::HttpClient *_httpClient,
                        const std::string_view method,
                        std::pair<const char *, Args> &&...args) {
    TgBot::HttpReqArg::Vec vec;
    std::string url(_bot_url);
    url += method;

    vec.reserve(sizeof...(Args));
    (
        [&vec](const std::pair<const char *, Args> arg) {
            using T = std::decay_t<Args>;
            if constexpr (detail::is_optional_v<T>) {
                if (static_cast<bool>(arg.second)) {
                    vec.emplace_back(putArg(arg.first, *arg.second));
                }
            } else if constexpr (detail::is_variant_v<T>) {
                if (arg.second.index() != std::variant_npos) {
                    std::visit(
                        [&](const auto &v) {
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
    if constexpr (kSendRequestDebug) {
        std::cout << "tgbot-cpp: Sending request: " << method << std::endl;
        for (const auto &arg : vec) {
            arg->print(std::cout) << std::endl;
        }
    }
    while (true) {
        try {
            std::string serverResponse = _httpClient->makeRequest(url, vec);

            if (!serverResponse.compare(0, 6, "<html>")) {
                std::string message =
                    "tgbot-cpp library have got html page instead of json "
                    "response. "
                    "Maybe you entered wrong bot token.";
                throw TgException(message,
                                  TgException::ErrorCode::HtmlResponse);
            }

            Json::Value result;
            Json::Reader reader;
            if (!reader.parse(serverResponse, result, false)) {
                if constexpr (kSendRequestDebug) {
                    std::cerr << "tgbot-cpp: Failed to parse response:"
                              << serverResponse << std::endl;
                }
                throw TgException(
                    "tgbot-cpp library can't parse json response.",
                    TgException::ErrorCode::InvalidJson);
            }

            if (result["ok"].asBool()) {
                return result["result"];
            } else {
                std::string message = result["description"].asString();
                int errorCode = result["error_code"].as<int>();

                throw TgException(
                    message, static_cast<TgException::ErrorCode>(errorCode));
            }
        } catch (const TgException &ex) {
            if constexpr (kSendRequestDebug) {
                std::cerr << "tgbot-cpp: Error: " << ex.what() << std::endl;
            }
            int max_retries = TgBot::HttpClient::kRequestMaxRetries;
            if ((max_retries >= 0) && (retries == max_retries)) {
                throw;
            } else {
                std::this_thread::sleep_for(TgBot::HttpClient::kRequestBackoff);
                retries++;
                continue;
            }
        }
    }
}

}  // namespace

namespace TgBot {

template <>
std::string putJSON<TgBot::Update::Types>(const TgBot::Update::Types &object) {
    Json::Value json;
    if (object & Update::Types::business_connection) {
        json.append("business_connection");
    }
    if (object & Update::Types::edited_business_message) {
        json.append("edited_business_message");
    }
    if (object & Update::Types::edited_channel_post) {
        json.append("edited_channel_post");
    }
    if (object & Update::Types::edited_message) {
        json.append("edited_message");
    }
    if (object & Update::Types::message) {
        json.append("message");
    }
    if (object & Update::Types::channel_post) {
        json.append("channel_post");
    }
    if (object & Update::Types::business_message) {
        json.append("business_message");
    }
    if (object & Update::Types::deleted_business_messages) {
        json.append("deleted_business_messages");
    }
    if (object & Update::Types::inline_query) {
        json.append("inline_query");
    }
    if (object & Update::Types::poll) {
        json.append("poll");
    }
    if (object & Update::Types::shipping_query) {
        json.append("shipping_query");
    }
    if (object & Update::Types::chosen_inline_result) {
        json.append("chosen_inline_result");
    }
    if (object & Update::Types::callback_query) {
        json.append("callback_query");
    }
    if (object & Update::Types::poll_answer) {
        json.append("poll_answer");
    }
    if (object & Update::Types::message_reaction) {
        json.append("message_reaction");
    }
    if (object & Update::Types::message_reaction_count) {
        json.append("message_reaction_count");
    }
    if (object & Update::Types::my_chat_member) {
        json.append("my_chat_member");
    }
    if (object & Update::Types::chat_member) {
        json.append("chat_member");
    }
    if (object & Update::Types::chat_join_request) {
        json.append("chat_join_request");
    }
    if (object & Update::Types::chat_boost) {
        json.append("chat_boost");
    }
    if (object & Update::Types::removed_chat_boost) {
        json.append("removed_chat_boost");
    }
    if (object & Update::Types::pre_checkout_query) {
        json.append("pre_checkout_query");
    }
    if (object & Update::Types::purchased_paid_media) {
        json.append("purchased_paid_media");
    }
    Json::FastWriter writer;
    return writer.write(json);
}

template <>
std::string putJSON<Api::ParseMode>(const Api::ParseMode &object) {
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
std::string putJSON<Api::PollType>(const Api::PollType &object) {
    switch (object) {
        case Api::PollType::regular:
            return "regular";
        case Api::PollType::quiz:
            return "quiz";
    }
    return {};
}

template <>
std::string putJSON<Api::StickerFormat>(const Api::StickerFormat &object) {
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
std::string putJSON<Api::ChatAction>(const Api::ChatAction &object) {
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
std::string putJSON<Sticker::Type>(const Sticker::Type &object) {
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

Api::Api(std::string token, HttpClient *httpClient, std::string url)
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
        .asBool();
}

bool Api::deleteWebhook(optional<bool> dropPendingUpdates) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteWebhook",
                       std::pair{"drop_pending_updates", dropPendingUpdates})
        .asBool();
}

WebhookInfo::Ptr Api::getWebhookInfo() const {
    const auto &p =
        sendRequest(_bot_api_baseurl, _httpClient, "getWebhookInfo");

    if (!p.isMember("url")) {
        return nullptr;
    }

    if (!p["url"].asString().empty()) {
        return parse<WebhookInfo>(p["url"]);
    } else {
        return nullptr;
    }
}

User::Ptr Api::getMe() const {
    return parse<User>(sendRequest(_bot_api_baseurl, _httpClient, "getMe"));
}

bool Api::logOut() const {
    return sendRequest(_bot_api_baseurl, _httpClient, "logOut").asBool();
}

bool Api::close() const {
    return sendRequest(_bot_api_baseurl, _httpClient, "close").asBool();
}

Message::Ptr Api::sendMessage(
    ChatIdType chatId, const std::string_view text,
    LinkPreviewOptions::Ptr linkPreviewOptions,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &entities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
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
        std::pair{"link_preview_options", std::move(linkPreviewOptions)}));
}

Message::Ptr Api::forwardMessage(ChatIdType chatId, ChatIdType fromChatId,
                                 std::int32_t messageId,
                                 optional<bool> disableNotification,
                                 optional<bool> protectContent,
                                 optional<std::int32_t> messageThreadId) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "forwardMessage",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"from_chat_id", std::move(fromChatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"protect_content", protectContent},
                    std::pair{"message_thread_id", messageThreadId}));
}

std::vector<MessageId::Ptr> Api::forwardMessages(
    ChatIdType chatId, ChatIdType fromChatId,
    const std::vector<std::int32_t> &messageIds,
    optional<std::int32_t> messageThreadId, optional<bool> disableNotification,
    optional<bool> protectContent) const {
    return parseArray<MessageId>(
        sendRequest(_bot_api_baseurl, _httpClient, "forwardMessages",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"from_chat_id", std::move(fromChatId)},
                    std::pair{"message_ids", messageIds},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"protect_content", protectContent}));
}

MessageId::Ptr Api::copyMessage(
    ChatIdType chatId, ChatIdType fromChatId, std::int32_t messageId,
    const optional<std::string_view> caption,
    const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    GenericReply::Ptr replyMarkup, optional<bool> protectContent,
    optional<std::int32_t> messageThreadId) const {
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
        std::pair{"message_thread_id", messageThreadId}));
}

std::vector<MessageId::Ptr> Api::copyMessages(
    ChatIdType chatId, ChatIdType fromChatId,
    const std::vector<std::int32_t> &messageIds,
    optional<std::int32_t> messageThreadId, optional<bool> disableNotification,
    optional<bool> protectContent, optional<bool> removeCaption) const {
    return parseArray<MessageId>(
        sendRequest(_bot_api_baseurl, _httpClient, "copyMessages",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"from_chat_id", std::move(fromChatId)},
                    std::pair{"message_ids", messageIds},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"protect_content", protectContent},
                    std::pair{"remove_caption", removeCaption}));
}

Message::Ptr Api::sendPhoto(
    ChatIdType chatId, FileHandleType photo,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "sendPhoto",
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
                    std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendAudio(
    ChatIdType chatId, FileHandleType audio,
    const optional<std::string_view> caption, optional<std::int32_t> duration,
    const optional<std::string_view> performer,
    const optional<std::string_view> title, FileHandleType thumbnail,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendAudio",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"audio", std::move(audio)},
        std::pair{"caption", caption}, std::pair{"duration", duration},
        std::pair{"performer", performer}, std::pair{"title", title},
        std::pair{"thumbnail", std::move(thumbnail)},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"parse_mode", parseMode},
        std::pair{"disable_notification", disableNotification},
        std::pair{"caption_entities", captionEntities},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendDocument(
    ChatIdType chatId, FileHandleType document, FileHandleType thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<bool> disableContentTypeDetection,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
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
                    std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendVideo(
    ChatIdType chatId, FileHandleType video, optional<bool> supportsStreaming,
    optional<std::int32_t> duration, optional<std::int32_t> width,
    optional<std::int32_t> height, FileHandleType thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendVideo",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"video", std::move(video)},
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
        std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendAnimation(
    ChatIdType chatId, FileHandleType animation,
    optional<std::int32_t> duration, optional<std::int32_t> width,
    optional<std::int32_t> height, FileHandleType thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendAnimation",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"animation", std::move(animation)}, std::pair{"duration", duration},
        std::pair{"width", width}, std::pair{"height", height},
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
        std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendVoice(
    ChatIdType chatId, FileHandleType voice,
    const optional<std::string_view> caption, optional<std::int32_t> duration,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendVoice",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"voice", std::move(voice)},
        std::pair{"caption", caption}, std::pair{"duration", duration},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"parse_mode", parseMode},
        std::pair{"disable_notification", disableNotification},
        std::pair{"caption_entities", captionEntities},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendVideoNote(
    ChatIdType chatId, FileHandleType videoNote,
    ReplyParameters::Ptr replyParameters, optional<bool> disableNotification,
    optional<std::int32_t> duration, optional<std::int32_t> length,
    FileHandleType thumbnail, GenericReply::Ptr replyMarkup,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
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
        std::pair{"business_connection_id", businessConnectionId}));
}

std::vector<Message::Ptr> Api::sendMediaGroup(
    ChatIdType chatId, const std::vector<InputMedia::Ptr> &media,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parseArray<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendMediaGroup",
        std::pair{"chat_id", std::move(chatId)}, std::pair{"media", media},
        std::pair{"disable_notification", disableNotification},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId}));
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
    const optional<std::string_view> businessConnectionId) const {
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
        std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::editMessageLiveLocation(
    float latitude, float longitude, ChatIdType chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    InlineKeyboardMarkup::Ptr replyMarkup,
    bounded_optional<float, 0, 1500> horizontalAccuracy,
    bounded_optional<std::int32_t, 1, 360> heading,
    bounded_optional<std::int32_t, 1, 100000> proximityAlertRadius) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageLiveLocation",
        std::pair{"latitude", latitude}, std::pair{"longitude", longitude},
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"message_id", messageId},
        std::pair{"inline_message_id", inlineMessageId},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"horizontal_accuracy", horizontalAccuracy},
        std::pair{"heading", heading},
        std::pair{"proximity_alert_radius", proximityAlertRadius}));
}

Message::Ptr Api::stopMessageLiveLocation(
    ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    InlineKeyboardMarkup::Ptr replyMarkup) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "stopMessageLiveLocation",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"inline_message_id", inlineMessageId},
                    std::pair{"reply_markup", std::move(replyMarkup)}));
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
    const optional<std::string_view> businessConnectionId) const {
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
        std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendContact(
    ChatIdType chatId, const std::string_view phoneNumber,
    const std::string_view firstName, const optional<std::string_view> lastName,
    const optional<std::string_view> vcard, optional<bool> disableNotification,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
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
                    std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendPoll(
    ChatIdType chatId, const std::string_view question,
    const std::vector<std::string> &options, optional<bool> disableNotification,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional_default<bool, true> isAnonymous,
    const optional_default<PollType, PollType::regular> type,
    optional_default<bool, false> allowsMultipleAnswers,
    optional<std::int32_t> correctOptionId,
    const optional<std::string_view> explanation,
    const optional<ParseMode> explanationParseMode,
    const std::vector<MessageEntity::Ptr> &explanationEntities,
    optional<std::int32_t> openPeriod,
    optional<std::chrono::system_clock::time_point> closeDate,
    optional<bool> isClosed, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
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
        std::pair{"business_connection_id", businessConnectionId}));
}

Message::Ptr Api::sendDice(
    ChatIdType chatId, optional<bool> disableNotification,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<std::string_view> emoji,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(
        sendRequest(_bot_api_baseurl, _httpClient, "sendDice",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"disable_notification", disableNotification},
                    std::pair{"reply_parameters", std::move(replyParameters)},
                    std::pair{"reply_markup", std::move(replyMarkup)},
                    std::pair{"emoji", emoji},
                    std::pair{"message_thread_id", messageThreadId},
                    std::pair{"protect_content", protectContent},
                    std::pair{"business_connection_id", businessConnectionId}));
}

bool Api::setMessageReaction(ChatIdType chatId,
                             optional<std::int32_t> messageId,
                             const std::vector<ReactionType::Ptr> &reaction,
                             optional<bool> isBig) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMessageReaction",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId},
                       std::pair{"reaction", reaction},
                       std::pair{"is_big", isBig})
        .asBool();
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
        .asBool();
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
        .asBool();
}

bool Api::unbanChatMember(ChatIdType chatId, std::int64_t userId,
                          optional<bool> onlyIfBanned) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unbanChatMember",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId},
                       std::pair{"only_if_banned", onlyIfBanned})
        .asBool();
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
        .asBool();
}

bool Api::promoteChatMember(
    ChatIdType chatId, std::int64_t userId, optional<bool> canChangeInfo,
    optional<bool> canPostMessages, optional<bool> canEditMessages,
    optional<bool> canDeleteMessages, optional<bool> canInviteUsers,
    optional<bool> canPinMessages, optional<bool> canPromoteMembers,
    optional<bool> isAnonymous, optional<bool> canManageChat,
    optional<bool> canManageVideoChats, optional<bool> canRestrictMembers,
    optional<bool> canManageTopics, optional<bool> canPostStories,
    optional<bool> canEditStories, optional<bool> canDeleteStories) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "promoteChatMember",
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
                       std::pair{"can_delete_stories", canDeleteStories})
        .asBool();
}

bool Api::setChatAdministratorCustomTitle(
    ChatIdType chatId, std::int64_t userId,
    const std::string_view customTitle) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setChatAdministratorCustomTitle",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId},
                       std::pair{"custom_title", customTitle})
        .asBool();
}

bool Api::banChatSenderChat(ChatIdType chatId,
                            std::int64_t senderChatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "banChatSenderChat",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"sender_chat_id", senderChatId})
        .asBool();
}

bool Api::unbanChatSenderChat(ChatIdType chatId,
                              std::int64_t senderChatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unbanChatSenderChat",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"sender_chat_id", senderChatId})
        .asBool();
}

bool Api::setChatPermissions(
    ChatIdType chatId, ChatPermissions::Ptr permissions,
    optional<bool> useIndependentChatPermissions) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatPermissions",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"permissions", std::move(permissions)},
                       std::pair{"use_independent_chat_permissions",
                                 useIndependentChatPermissions})
        .asBool();
}

std::string Api::exportChatInviteLink(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "exportChatInviteLink",
                       std::pair{"chat_id", std::move(chatId)})
        .asString();
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
        .asBool();
}

bool Api::declineChatJoinRequest(ChatIdType chatId, std::int64_t userId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "declineChatJoinRequest",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"user_id", userId})
        .asBool();
}

bool Api::setChatPhoto(ChatIdType chatId, InputFile::Ptr photo) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatPhoto",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"photo", std::move(photo)})
        .asBool();
}

bool Api::deleteChatPhoto(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteChatPhoto",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
}

bool Api::setChatTitle(ChatIdType chatId, const std::string_view title) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatTitle",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"title", title})
        .asBool();
}

bool Api::setChatDescription(ChatIdType chatId,
                             const std::string_view description) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setChatDescription",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"description", description})
        .asBool();
}

bool Api::pinChatMessage(ChatIdType chatId, std::int32_t messageId,
                         optional<bool> disableNotification) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "pinChatMessage",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId},
                       std::pair{"disable_notification", disableNotification})
        .asBool();
}

bool Api::unpinChatMessage(ChatIdType chatId,
                           optional<std::int32_t> messageId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unpinChatMessage",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId})
        .asBool();
}

bool Api::unpinAllChatMessages(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unpinAllChatMessages",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
}

bool Api::leaveChat(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "leaveChat",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
}

Chat::Ptr Api::getChat(ChatIdType chatId) const {
    return parse<Chat>(sendRequest(_bot_api_baseurl, _httpClient, "getChat",
                                   std::pair{"chat_id", std::move(chatId)}));
}

std::vector<ChatMember::Ptr> Api::getChatAdministrators(
    ChatIdType chatId) const {
    return parseArray<ChatMember>(
        sendRequest(_bot_api_baseurl, _httpClient, "getChatAdministrators",
                    std::pair{"chat_id", std::move(chatId)}));
}

int32_t Api::getChatMemberCount(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "getChatMemberCount",
                       std::pair{"chat_id", std::move(chatId)})
        .asInt();
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
        .asBool();
}

bool Api::deleteChatStickerSet(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteChatStickerSet",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
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
    return sendRequest(_bot_api_baseurl, _httpClient, "editForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId},
                       std::pair{"name", name},
                       std::pair{"icon_custom_emoji_id", std::move(iconCustomEmojiId)})
        .asBool();
}

bool Api::closeForumTopic(ChatIdType chatId,
                          std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "closeForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .asBool();
}

bool Api::reopenForumTopic(ChatIdType chatId,
                           std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "reopenForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .asBool();
}

bool Api::deleteForumTopic(ChatIdType chatId,
                           std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .asBool();
}

bool Api::unpinAllForumTopicMessages(ChatIdType chatId,
                                     std::int32_t messageThreadId) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "unpinAllForumTopicMessages",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_thread_id", messageThreadId})
        .asBool();
}

bool Api::editGeneralForumTopic(ChatIdType chatId, std::string name) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "editGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"name", std::move(name)})
        .asBool();
}

bool Api::closeGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "closeGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
}

bool Api::reopenGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "reopenGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
}

bool Api::hideGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "hideGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
}

bool Api::unhideGeneralForumTopic(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "unhideGeneralForumTopic",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
}

bool Api::unpinAllGeneralForumTopicMessages(ChatIdType chatId) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "unpinAllGeneralForumTopicMessages",
                       std::pair{"chat_id", std::move(chatId)})
        .asBool();
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
        .asBool();
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

bool Api::setMyCommands(const std::vector<BotCommand::Ptr> &commands,
                        BotCommandScope::Ptr scope,
                        const optional<LanguageCode> languageCode) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setMyCommands",
                       std::pair{"commands", commands},
                       std::pair{"scope", std::move(scope)},
                       std::pair{"language_code", languageCode})
        .asBool();
}

bool Api::deleteMyCommands(BotCommandScope::Ptr scope,
                           const optional<LanguageCode> languageCode) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteMyCommands",
                       std::pair{"scope", std::move(scope)},
                       std::pair{"language_code", languageCode})
        .asBool();
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
        .asBool();
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
        .asBool();
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
        .asBool();
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
        .asBool();
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
        .asBool();
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
    const std::vector<MessageEntity::Ptr> &entities) const {
    const auto p = sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageText",
        std::pair{"text", text}, std::pair{"chat_id", std::move(chatId)},
        std::pair{"message_id", messageId},
        std::pair{"inline_message_id", inlineMessageId},
        std::pair{"parse_mode", parseMode},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"entities", entities},
        std::pair{"link_preview", std::move(linkPreviewOptions)});
    if (p.isMember("message_id")) {
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
    const std::vector<MessageEntity::Ptr> &captionEntities) const {
    const auto p = sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageCaption",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"message_id", messageId}, std::pair{"caption", caption},
        std::pair{"inline_message_id", inlineMessageId},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"parse_mode", parseMode},
        std::pair{"caption_entities", captionEntities});
    if (p.isMember("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageMedia(
    InputMedia::Ptr media, ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup) const {
    const auto &p = sendRequest(
        _bot_api_baseurl, _httpClient, "editMessageMedia",
        std::pair{"media", std::move(media)}, std::pair{"chat_id", std::move(chatId)},
        std::pair{"message_id", messageId},
        std::pair{"inline_message_id", inlineMessageId},
        std::pair{"reply_markup", std::move(replyMarkup)});
    if (p.isMember("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageReplyMarkup(
    ChatIdType chatId, optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup) const {
    const auto &p =
        sendRequest(_bot_api_baseurl, _httpClient, "editMessageReplyMarkup",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"inline_message_id", inlineMessageId},
                    std::pair{"reply_markup", std::move(replyMarkup)});
    if (p.isMember("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Poll::Ptr Api::stopPoll(ChatIdType chatId, std::int64_t messageId,
                        InlineKeyboardMarkup::Ptr replyMarkup) const {
    return parse<Poll>(
        sendRequest(_bot_api_baseurl, _httpClient, "stopPoll",
                    std::pair{"chat_id", std::move(chatId)},
                    std::pair{"message_id", messageId},
                    std::pair{"reply_markup", std::move(replyMarkup)}));
}

bool Api::deleteMessage(ChatIdType chatId, std::int32_t messageId) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteMessage",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_id", messageId})
        .asBool();
}

bool Api::deleteMessages(ChatIdType chatId,
                         const std::vector<std::int32_t> &messageIds) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteMessages",
                       std::pair{"chat_id", std::move(chatId)},
                       std::pair{"message_ids", messageIds})
        .asBool();
}

Message::Ptr Api::sendSticker(
    ChatIdType chatId, FileHandleType sticker,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<bool> disableNotification, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent, const optional<std::string_view> emoji,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendSticker",
        std::pair{"chat_id", std::move(chatId)},
        std::pair{"sticker", std::move(sticker)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"reply_params", std::move(replyParameters)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent}, std::pair{"emoji", emoji},
        std::pair{"business_connection_id", businessConnectionId}));
}

StickerSet::Ptr Api::getStickerSet(const std::string_view name) const {
    return parse<StickerSet>(sendRequest(_bot_api_baseurl, _httpClient,
                                         "getStickerSet",
                                         std::pair{"name", name}));
}

std::vector<Sticker::Ptr> Api::getCustomEmojiStickers(
    const std::vector<std::string> &customEmojiIds) const {
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
    const std::vector<InputSticker::Ptr> &stickers,
    optional_default<Sticker::Type, Sticker::Type::Regular> stickerType,
    optional<bool> needsRepainting) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "createNewStickerSet",
                       std::pair{"user_id", userId}, std::pair{"name", name},
                       std::pair{"title", title},
                       std::pair{"stickers", stickers},
                       std::pair{"sticker_type", stickerType},
                       std::pair{"needs_repainting", needsRepainting})
        .asBool();
}

bool Api::addStickerToSet(std::int64_t userId, const std::string_view name,
                          InputSticker::Ptr sticker) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "addStickerToSet",
                       std::pair{"user_id", userId}, std::pair{"name", name},
                       std::pair{"sticker", std::move(sticker)})
        .asBool();
}

bool Api::setStickerPositionInSet(const std::string_view sticker,
                                  std::int32_t position) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerPositionInSet",
                       std::pair{"sticker", sticker},
                       std::pair{"position", position})
        .asBool();
}

bool Api::deleteStickerFromSet(const std::string_view sticker) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteStickerFromSet",
                       std::pair{"sticker", sticker})
        .asBool();
}

bool Api::replaceStickerInSet(std::int64_t userId, const std::string_view name,
                              const std::string_view oldSticker,
                              InputSticker::Ptr sticker) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "replaceStickerInSet",
                       std::pair{"user_id", userId}, std::pair{"name", name},
                       std::pair{"old_sticker", oldSticker},
                       std::pair{"sticker", std::move(sticker)})
        .asBool();
}

bool Api::setStickerEmojiList(const std::string_view sticker,
                              const std::vector<std::string> &emojiList) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerEmojiList",
                       std::pair{"sticker", sticker},
                       std::pair{"emoji_list", emojiList})
        .asBool();
}

bool Api::setStickerKeywords(const std::string_view sticker,
                             const std::vector<std::string> &keywords) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerKeywords",
                       std::pair{"sticker", sticker},
                       std::pair{"keywords", keywords})
        .asBool();
}

bool Api::setStickerMaskPosition(const std::string_view sticker,
                                 MaskPosition::Ptr maskPosition) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerMaskPosition",
                       std::pair{"sticker", sticker},
                       std::pair{"mask_position", std::move(maskPosition)})
        .asBool();
}

bool Api::setStickerSetTitle(const std::string_view name,
                             const std::string_view title) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerSetTitle",
                       std::pair{"name", name}, std::pair{"title", title})
        .asBool();
}

bool Api::setStickerSetThumbnail(const std::string_view name,
                                 std::int64_t userId,
                                 const StickerFormat format,
                                 FileHandleType thumbnail) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setStickerSetThumbnail",
                       std::pair{"name", name}, std::pair{"user_id", userId},
                       std::pair{"sticker_format", format},
                       std::pair{"thumbnail", std::move(thumbnail)})
        .asBool();
}

bool Api::setCustomEmojiStickerSetThumbnail(
    const std::string_view name,
    const optional<std::string_view> customEmojiId) const {
    return sendRequest(_bot_api_baseurl, _httpClient,
                       "setCustomEmojiStickerSetThumbnail",
                       std::pair{"name", name},
                       std::pair{"custom_emoji_id", customEmojiId})
        .asBool();
}

bool Api::deleteStickerSet(const std::string_view name) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "deleteStickerSet",
                       std::pair{"name", name})
        .asBool();
}

bool Api::answerInlineQuery(const std::string_view inlineQueryId,
                            const std::vector<InlineQueryResult::Ptr> &results,
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
        .asBool();
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
    const std::vector<LabeledPrice::Ptr> &prices,
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
    const std::vector<std::int32_t> &suggestedTipAmounts,
    const optional<std::string_view> startParameter,
    optional<bool> protectContent) const {
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
        std::pair{"reply_parameters", std::move(replyParameters)}));
}

std::string Api::createInvoiceLink(
    const std::string_view title, const std::string_view description,
    const std::string_view payload, const std::string_view providerToken,
    const std::string_view currency,
    const std::vector<LabeledPrice::Ptr> &prices,
    optional<std::int32_t> maxTipAmount,
    const std::vector<std::int32_t> &suggestedTipAmounts,
    const optional<std::string_view> providerData,
    const optional<std::string_view> photoUrl, optional<std::int32_t> photoSize,
    optional<std::int32_t> photoWidth, optional<std::int32_t> photoHeight,
    optional<bool> needName, optional<bool> needPhoneNumber,
    optional<bool> needEmail, optional<bool> needShippingAddress,
    optional<bool> sendPhoneNumberToProvider,
    optional<bool> sendEmailToProvider, optional<bool> isFlexible) const {
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
               std::pair{"is_flexible", isFlexible})
        .asString();
}

bool Api::answerShippingQuery(
    const std::string_view shippingQueryId, bool ok,
    const std::vector<ShippingOption::Ptr> &shippingOptions,
    const optional<std::string_view> errorMessage) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "answerShippingQuery",
                       std::pair{"shipping_query_id", shippingQueryId},
                       std::pair{"ok", ok},
                       std::pair{"shipping_options", shippingOptions},
                       std::pair{"error_message", errorMessage})
        .asBool();
}

bool Api::answerPreCheckoutQuery(
    const std::string_view preCheckoutQueryId, bool ok,
    const optional<std::string_view> errorMessage) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "answerPreCheckoutQuery",
                       std::pair{"pre_checkout_query_id", preCheckoutQueryId},
                       std::pair{"ok", ok},
                       std::pair{"error_message", errorMessage})
        .asBool();
}

bool Api::setPassportDataErrors(
    std::int64_t userId,
    const std::vector<PassportElementError::Ptr> &errors) const {
    return sendRequest(_bot_api_baseurl, _httpClient, "setPassportDataErrors",
                       std::pair{"user_id", userId},
                       std::pair{"errors", errors})
        .asBool();
}

Message::Ptr Api::sendGame(
    std::int64_t chatId, const std::string_view gameShortName,
    ReplyParameters::Ptr replyParameters, InlineKeyboardMarkup::Ptr replyMarkup,
    optional<bool> disableNotification, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        _bot_api_baseurl, _httpClient, "sendGame", std::pair{"chat_id", chatId},
        std::pair{"game_short_name", gameShortName},
        std::pair{"reply_parameters", std::move(replyParameters)},
        std::pair{"reply_markup", std::move(replyMarkup)},
        std::pair{"disable_notification", disableNotification},
        std::pair{"message_thread_id", messageThreadId},
        std::pair{"protect_content", protectContent},
        std::pair{"business_connection_id", businessConnectionId}));
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
                              const HttpReqArg::Vec &args) const {
    std::string url(_url);
    url += "/file/bot";
    url += _token;
    url += "/";
    url += filePath;

    return _httpClient->makeRequest(url, args);
}

bool Api::blockedByUser(std::int64_t chatId) const {
    bool isBotBlocked = false;

    try {
        sendChatAction(chatId, ChatAction::typing);

    } catch (std::exception &e) {
        std::string error = e.what();

        if (error.find("Forbidden: bot was blocked by the user") ==
            std::string::npos) {
            isBotBlocked = true;
        }
    }

    return isBotBlocked;
}

}  // namespace TgBot
