#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <tgbot/ApiImpl.h>
#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>
#include <tgbot/tools/StringTools.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <variant>

#include "tgbot/net/HttpReqArg.h"
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
}

template <>
std::string putJSON<Api::PollType>(const Api::PollType &object) {
    switch (object) {
        case Api::PollType::regular:
            return "regular";
        case Api::PollType::quiz:
            return "quiz";
    }
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
    }
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
}
}  // namespace TgBot

namespace {

template <typename T>
auto putArg(T arg) {
    if constexpr (std::is_same_v<T, std::string_view>) {
        return std::string(arg);
    } else if constexpr (std::is_same_v<T, TgBot::InputFile::Ptr> ||
                         detail::is_primitive_v<T>) {
        return arg;  // HttpReqArg ctor can handle
    } else if constexpr (std::is_same_v<
                             T, std::chrono::system_clock::time_point>) {
        return std::chrono::system_clock::to_time_t(arg);
    } else {
        return TgBot::putJSON(arg);
    }
}

template <typename... Args>
std::vector<TgBot::HttpReqArg> build(std::pair<const char *, Args> &&...args) {
    std::vector<TgBot::HttpReqArg> vec;
    vec.reserve(sizeof...(Args));
    (
        [&vec](const std::pair<const char *, Args> arg) {
            using T = std::decay_t<Args>;
            if constexpr (detail::is_optional_v<T>) {
                if (static_cast<bool>(arg.second)) {
                    vec.emplace_back(arg.first, putArg(*arg.second));
                }
            } else if constexpr (detail::is_variant_v<T>) {
                if (arg.second.index() != std::variant_npos) {
                    vec.emplace_back(arg.first, arg.second);
                }
            } else if constexpr (detail::is_shared_ptr_v<T>) {
                if (arg.second != nullptr) {
                    vec.emplace_back(arg.first, putArg(arg.second));
                }
            } else if constexpr (detail::is_vector_v<T>) {
                if (arg.second.size() != 0) {
                    vec.emplace_back(arg.first, putArg(arg.second));
                }
            } else {
                vec.emplace_back(arg.first, putArg(arg.second));
            }
        }(args),
        ...);
    return vec;
}
}  // namespace

namespace TgBot {

ApiImpl::ApiImpl(std::string token, HttpClient *httpClient, std::string url)
    : _httpClient(httpClient), _token(std::move(token)), _url(std::move(url)) {}

std::vector<Update::Ptr> ApiImpl::getUpdates(
    optional<std::int32_t> offset,
    bounded_optional_default<std::int32_t, 0, 100, 100> limit,
    optional_default<std::int32_t, 0> timeout,
    const optional<Update::Types> allowedUpdates) const {
    return parseArray<Update>(sendRequest(
        "getUpdates",
        build(std::pair{"offset", offset}, std::pair{"limit", limit},
              std::pair{"timeout", timeout},
              std::pair{"allowed_updates", allowedUpdates})));
}

bool ApiImpl::setWebhook(
    const std::string_view url, InputFile::Ptr certificate,
    bounded_optional_default<std::int32_t, 1, 100, 40> maxConnections,
    const optional<Update::Types> allowedUpdates,
    const optional<std::string_view> ipAddress,
    optional<bool> dropPendingUpdates,
    const optional<std::string_view> secretToken) const {
    return sendRequest(
               "setWebhook",
               build(std::pair{"url", url},
                     std::pair{"certificate", certificate},
                     std::pair{"max_connections", maxConnections},
                     std::pair{"allowed_updates", allowedUpdates},
                     std::pair{"ip_address", ipAddress},
                     std::pair{"drop_pending_updates", dropPendingUpdates},
                     std::pair{"secret_token", secretToken}))
        .asBool();
}

bool ApiImpl::deleteWebhook(optional<bool> dropPendingUpdates) const {
    auto args = build(std::pair{"drop_pending_updates", dropPendingUpdates});

    return sendRequest("deleteWebhook", std::move(args)).asBool();
}

WebhookInfo::Ptr ApiImpl::getWebhookInfo() const {
    const auto &p = sendRequest("getWebhookInfo");

    if (!p.isMember("url")) {
        return nullptr;
    }

    if (!p["url"].asString().empty()) {
        return parse<WebhookInfo>(p["url"]);
    } else {
        return nullptr;
    }
}

User::Ptr ApiImpl::getMe() const { return parse<User>(sendRequest("getMe")); }

bool ApiImpl::logOut() const { return sendRequest("logOut").asBool(); }

bool ApiImpl::close() const { return sendRequest("close").asBool(); }

Message::Ptr ApiImpl::sendMessage(
    std::variant<std::int64_t, std::string> chatId, const std::string_view text,
    LinkPreviewOptions::Ptr linkPreviewOptions,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &entities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendMessage",
        build(std::pair{"chat_id", chatId}, std::pair{"text", text},
              std::pair{"parse_mode", parseMode},
              std::pair{"disable_notification", disableNotification},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"entities", entities},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId},
              std::pair{"link_preview_options", linkPreviewOptions})));
}

Message::Ptr ApiImpl::forwardMessage(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<std::int64_t, std::string> fromChatId, std::int32_t messageId,
    optional<bool> disableNotification, optional<bool> protectContent,
    optional<std::int32_t> messageThreadId) const {
    return parse<Message>(sendRequest(
        "forwardMessage",
        build(std::pair{"chat_id", chatId},
              std::pair{"from_chat_id", fromChatId},
              std::pair{"message_id", messageId},
              std::pair{"disable_notification", disableNotification},
              std::pair{"protect_content", protectContent},
              std::pair{"message_thread_id", messageThreadId})));
}

std::vector<MessageId::Ptr> ApiImpl::forwardMessages(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<std::int64_t, std::string> fromChatId,
    const std::vector<std::int32_t> &messageIds,
    optional<std::int32_t> messageThreadId, optional<bool> disableNotification,
    optional<bool> protectContent) const {
    return parseArray<MessageId>(sendRequest(
        "forwardMessages",
        build(std::pair{"chat_id", chatId},
              std::pair{"from_chat_id", fromChatId},
              std::pair{"message_ids", messageIds},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"disable_notification", disableNotification},
              std::pair{"protect_content", protectContent})));
}

MessageId::Ptr ApiImpl::copyMessage(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<std::int64_t, std::string> fromChatId, std::int32_t messageId,
    const optional<std::string_view> caption,
    const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    GenericReply::Ptr replyMarkup, optional<bool> protectContent,
    optional<std::int32_t> messageThreadId) const {
    return parse<MessageId>(sendRequest(
        "copyMessage",
        build(std::pair{"chat_id", chatId},
              std::pair{"from_chat_id", fromChatId},
              std::pair{"message_id", messageId}, std::pair{"caption", caption},
              std::pair{"parse_mode", parseMode},
              std::pair{"caption_entities", captionEntities},
              std::pair{"disable_notification", disableNotification},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"protect_content", protectContent},
              std::pair{"message_thread_id", messageThreadId})));
}

std::vector<MessageId::Ptr> ApiImpl::copyMessages(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<std::int64_t, std::string> fromChatId,
    const std::vector<std::int32_t> &messageIds,
    optional<std::int32_t> messageThreadId, optional<bool> disableNotification,
    optional<bool> protectContent, optional<bool> removeCaption) const {
    return parseArray<MessageId>(sendRequest(
        "copyMessages",
        build(std::pair{"chat_id", chatId},
              std::pair{"from_chat_id", fromChatId},
              std::pair{"message_ids", messageIds},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"disable_notification", disableNotification},
              std::pair{"protect_content", protectContent},
              std::pair{"remove_caption", removeCaption})));
}

Message::Ptr ApiImpl::sendPhoto(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> photo,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendPhoto",
        build(std::pair{"chat_id", chatId}, std::pair{"photo", photo},
              std::pair{"caption", caption},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"parse_mode", parseMode},
              std::pair{"disable_notification", disableNotification},
              std::pair{"caption_entities", putJSON(captionEntities)},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"has_spoiler", hasSpoiler},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendAudio(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> audio,
    const optional<std::string_view> caption, optional<std::int32_t> duration,
    const optional<std::string_view> performer,
    const optional<std::string_view> title,
    std::variant<InputFile::Ptr, std::string> thumbnail,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendAudio",
        build(std::pair{"chat_id", chatId}, std::pair{"audio", audio},
              std::pair{"caption", caption}, std::pair{"duration", duration},
              std::pair{"performer", performer}, std::pair{"title", title},
              std::pair{"thumbnail", thumbnail},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"parse_mode", parseMode},
              std::pair{"disable_notification", disableNotification},
              std::pair{"caption_entities", putJSON(captionEntities)},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendDocument(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> document,
    std::variant<InputFile::Ptr, std::string> thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<bool> disableContentTypeDetection,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendDocument",
        build(std::pair{"chat_id", chatId}, std::pair{"document", document},
              std::pair{"thumbnail", thumbnail}, std::pair{"caption", caption},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"parse_mode", parseMode},
              std::pair{"disable_notification", disableNotification},
              std::pair{"caption_entities", captionEntities},
              std::pair{"disable_content_type_detection",
                        disableContentTypeDetection},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendVideo(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> video,
    optional<bool> supportsStreaming, optional<std::int32_t> duration,
    optional<std::int32_t> width, optional<std::int32_t> height,
    std::variant<InputFile::Ptr, std::string> thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendVideo",
        build(std::pair{"chat_id", chatId}, std::pair{"video", video},
              std::pair{"supports_streaming", supportsStreaming},
              std::pair{"duration", duration}, std::pair{"width", width},
              std::pair{"height", height}, std::pair{"thumbnail", thumbnail},
              std::pair{"caption", caption},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"parse_mode", parseMode},
              std::pair{"disable_notification", disableNotification},
              std::pair{"caption_entities", captionEntities},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"has_spoiler", hasSpoiler},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendAnimation(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> animation,
    optional<std::int32_t> duration, optional<std::int32_t> width,
    optional<std::int32_t> height,
    std::variant<InputFile::Ptr, std::string> thumbnail,
    const optional<std::string_view> caption,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    optional<bool> hasSpoiler,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendAnimation",
        build(std::pair{"chat_id", chatId}, std::pair{"animation", animation},
              std::pair{"duration", duration}, std::pair{"width", width},
              std::pair{"height", height}, std::pair{"thumbnail", thumbnail},
              std::pair{"caption", caption},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"parse_mode", parseMode},
              std::pair{"disable_notification", disableNotification},
              std::pair{"caption_entities", captionEntities},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"has_spoiler", hasSpoiler},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendVoice(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> voice,
    const optional<std::string_view> caption, optional<std::int32_t> duration,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    const optional<ParseMode> parseMode, optional<bool> disableNotification,
    const std::vector<MessageEntity::Ptr> &captionEntities,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendVoice",
        build(std::pair{"chat_id", chatId}, std::pair{"voice", voice},
              std::pair{"caption", caption}, std::pair{"duration", duration},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"parse_mode", parseMode},
              std::pair{"disable_notification", disableNotification},
              std::pair{"caption_entities", captionEntities},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendVideoNote(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> videoNote,
    ReplyParameters::Ptr replyParameters, optional<bool> disableNotification,
    optional<std::int32_t> duration, optional<std::int32_t> length,
    std::variant<InputFile::Ptr, std::string> thumbnail,
    GenericReply::Ptr replyMarkup, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendVideoNote",
        build(std::pair{"chat_id", chatId}, std::pair{"video_note", videoNote},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"disable_notification", disableNotification},
              std::pair{"duration", duration}, std::pair{"length", length},
              std::pair{"thumbnail", thumbnail},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

std::vector<Message::Ptr> ApiImpl::sendMediaGroup(
    std::variant<std::int64_t, std::string> chatId,
    const std::vector<InputMedia::Ptr> &media,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parseArray<Message>(sendRequest(
        "sendMediaGroup",
        build(std::pair{"chat_id", chatId}, std::pair{"media", media},
              std::pair{"disable_notification", disableNotification},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendLocation(
    std::variant<std::int64_t, std::string> chatId, float latitude,
    float longitude, bounded_optional<std::int32_t, 60, 86400> livePeriod,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<bool> disableNotification,
    bounded_optional<float, 0, 1500> horizontalAccuracy,
    optional<std::int32_t> heading,
    bounded_optional<std::int32_t, 1, 100000> proximityAlertRadius,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendLocation",
        build(std::pair{"chat_id", chatId}, std::pair{"latitude", latitude},
              std::pair{"longitude", longitude},
              std::pair{"live_period", livePeriod},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"disable_notification", disableNotification},
              std::pair{"horizontal_accuracy", horizontalAccuracy},
              std::pair{"heading", heading},
              std::pair{"proximity_alert_radius", proximityAlertRadius},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::editMessageLiveLocation(
    float latitude, float longitude,
    std::variant<std::int64_t, std::string> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    InlineKeyboardMarkup::Ptr replyMarkup,
    bounded_optional<float, 0, 1500> horizontalAccuracy,
    bounded_optional<std::int32_t, 1, 360> heading,
    bounded_optional<std::int32_t, 1, 100000> proximityAlertRadius) const {
    return parse<Message>(sendRequest(
        "editMessageLiveLocation",
        build(std::pair{"latitude", latitude},
              std::pair{"longitude", longitude}, std::pair{"chat_id", chatId},
              std::pair{"message_id", messageId},
              std::pair{"inline_message_id", inlineMessageId},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"horizontal_accuracy", horizontalAccuracy},
              std::pair{"heading", heading},
              std::pair{"proximity_alert_radius", proximityAlertRadius})));
}

Message::Ptr ApiImpl::stopMessageLiveLocation(
    std::variant<std::int64_t, std::string> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    InlineKeyboardMarkup::Ptr replyMarkup) const {
    return parse<Message>(sendRequest(
        "stopMessageLiveLocation",
        build(std::pair{"chat_id", chatId}, std::pair{"message_id", messageId},
              std::pair{"inline_message_id", inlineMessageId},
              std::pair{"reply_markup", replyMarkup})));
}

Message::Ptr ApiImpl::sendVenue(
    std::variant<std::int64_t, std::string> chatId, float latitude,
    float longitude, const std::string_view title,
    const std::string_view address,
    const optional<std::string_view> foursquareId,
    const optional<std::string_view> foursquareType,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    GenericReply::Ptr replyMarkup,
    const optional<std::string_view> googlePlaceId,
    const optional<std::string_view> googlePlaceType,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendVenue",
        build(std::pair{"chat_id", chatId}, std::pair{"latitude", latitude},
              std::pair{"longitude", longitude}, std::pair{"title", title},
              std::pair{"address", address},
              std::pair{"foursquare_id", foursquareId},
              std::pair{"foursquare_type", foursquareType},
              std::pair{"disable_notification", disableNotification},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"google_place_id", googlePlaceId},
              std::pair{"google_place_type", googlePlaceType},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendContact(
    std::variant<std::int64_t, std::string> chatId,
    const std::string_view phoneNumber, const std::string_view firstName,
    const optional<std::string_view> lastName,
    const optional<std::string_view> vcard, optional<bool> disableNotification,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendContact",
        build(std::pair{"chat_id", chatId},
              std::pair{"phone_number", phoneNumber},
              std::pair{"first_name", firstName},
              std::pair{"last_name", lastName}, std::pair{"vcard", vcard},
              std::pair{"disable_notification", disableNotification},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendPoll(
    std::variant<std::int64_t, std::string> chatId,
    const std::string_view question, const std::vector<std::string> &options,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    GenericReply::Ptr replyMarkup, optional_default<bool, true> isAnonymous,
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
        "sendPoll",
        build(std::pair{"chat_id", chatId}, std::pair{"question", question},
              std::pair{"options", options},
              std::pair{"disable_notification", disableNotification},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"is_anonymous", isAnonymous}, std::pair{"type", type},
              std::pair{"allows_multiple_answers", allowsMultipleAnswers},
              std::pair{"correct_option_id", correctOptionId},
              std::pair{"explanation", explanation},
              std::pair{"explanation_parse_mode", explanationParseMode},
              std::pair{"explanation_entities", explanationEntities},
              std::pair{"open_period", openPeriod},
              std::pair{"close_date", closeDate},
              std::pair{"is_closed", isClosed},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::sendDice(
    std::variant<std::int64_t, std::string> chatId,
    optional<bool> disableNotification, ReplyParameters::Ptr replyParameters,
    GenericReply::Ptr replyMarkup, const optional<std::string_view> emoji,
    optional<std::int32_t> messageThreadId, optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendDice",
        build(std::pair{"chat_id", chatId},
              std::pair{"disable_notification", disableNotification},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup}, std::pair{"emoji", emoji},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

bool ApiImpl::setMessageReaction(std::variant<std::int64_t, std::string> chatId,
                                 optional<std::int32_t> messageId,
                                 const std::vector<ReactionType::Ptr> &reaction,
                                 optional<bool> isBig) const {
    return sendRequest("setMessageReaction",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_id", messageId},
                             std::pair{"reaction", reaction},
                             std::pair{"is_big", isBig}))
        .asBool();
}

bool ApiImpl::sendChatAction(
    std::int64_t chatId, const ChatAction action,
    optional<std::int32_t> messageThreadId,
    const optional<std::string_view> businessConnectionId) const {
    return sendRequest(
               "sendChatAction",
               build(std::pair{"chat_id", chatId}, std::pair{"action", action},
                     std::pair{"message_thread_id", messageThreadId},
                     std::pair{"business_connection_id", businessConnectionId}))
        .asBool();
}

UserProfilePhotos::Ptr ApiImpl::getUserProfilePhotos(
    std::int64_t userId, optional<std::int32_t> offset,
    bounded_optional_default<std::int32_t, 1, 100, 100> limit) const {
    return parse<UserProfilePhotos>(sendRequest(
        "getUserProfilePhotos",
        build(std::pair{"user_id", userId}, std::pair{"offset", offset},
              std::pair{"limit", limit})));
}

File::Ptr ApiImpl::getFile(const std::string_view fileId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("file_id", fileId);

    return parse<File>(sendRequest("getFile", args));
}

bool ApiImpl::banChatMember(
    std::variant<std::int64_t, std::string> chatId, std::int64_t userId,
    optional<std::chrono::system_clock::time_point> untilDate,
    optional<bool> revokeMessages) const {
    return sendRequest(
               "banChatMember",
               build(std::pair{"chat_id", chatId}, std::pair{"user_id", userId},
                     std::pair{"until_date", untilDate},
                     std::pair{"revoke_messages", revokeMessages}))
        .asBool();
}

bool ApiImpl::unbanChatMember(std::variant<std::int64_t, std::string> chatId,
                              std::int64_t userId,
                              optional<bool> onlyIfBanned) const {
    return sendRequest(
               "unbanChatMember",
               build(std::pair{"chat_id", chatId}, std::pair{"user_id", userId},
                     std::pair{"only_if_banned", onlyIfBanned}))
        .asBool();
}

bool ApiImpl::restrictChatMember(
    std::variant<std::int64_t, std::string> chatId, std::int64_t userId,
    ChatPermissions::Ptr permissions,
    optional<std::chrono::system_clock::time_point> untilDate,
    optional<bool> useIndependentChatPermissions) const {
    return sendRequest(
               "restrictChatMember",
               build(std::pair{"chat_id", chatId}, std::pair{"user_id", userId},
                     std::pair{"permissions", permissions},
                     std::pair{"until_date", untilDate},
                     std::pair{"use_independent_chat_permissions",
                               useIndependentChatPermissions}))
        .asBool();
}

bool ApiImpl::promoteChatMember(
    std::variant<std::int64_t, std::string> chatId, std::int64_t userId,
    optional<bool> canChangeInfo, optional<bool> canPostMessages,
    optional<bool> canEditMessages, optional<bool> canDeleteMessages,
    optional<bool> canInviteUsers, optional<bool> canPinMessages,
    optional<bool> canPromoteMembers, optional<bool> isAnonymous,
    optional<bool> canManageChat, optional<bool> canManageVideoChats,
    optional<bool> canRestrictMembers, optional<bool> canManageTopics,
    optional<bool> canPostStories, optional<bool> canEditStories,
    optional<bool> canDeleteStories) const {
    return sendRequest(
               "promoteChatMember",
               build(std::pair{"chat_id", chatId}, std::pair{"user_id", userId},
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
                     std::pair{"can_delete_stories", canDeleteStories}))
        .asBool();
}

bool ApiImpl::setChatAdministratorCustomTitle(
    std::variant<std::int64_t, std::string> chatId, std::int64_t userId,
    const std::string_view customTitle) const {
    return sendRequest(
               "setChatAdministratorCustomTitle",
               build(std::pair{"chat_id", chatId}, std::pair{"user_id", userId},
                     std::pair{"custom_title", customTitle}))
        .asBool();
}

bool ApiImpl::banChatSenderChat(std::variant<std::int64_t, std::string> chatId,
                                std::int64_t senderChatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("sender_chat_id", senderChatId);

    return sendRequest("banChatSenderChat", args).asBool();
}

bool ApiImpl::unbanChatSenderChat(
    std::variant<std::int64_t, std::string> chatId,
    std::int64_t senderChatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("sender_chat_id", senderChatId);

    return sendRequest("unbanChatSenderChat", args).asBool();
}

bool ApiImpl::setChatPermissions(
    std::variant<std::int64_t, std::string> chatId,
    ChatPermissions::Ptr permissions,
    optional<bool> useIndependentChatPermissions) const {
    return sendRequest("setChatPermissions",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"permissions", permissions},
                             std::pair{"use_independent_chat_permissions",
                                       useIndependentChatPermissions}))
        .asBool();
}

std::string ApiImpl::exportChatInviteLink(
    std::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return sendRequest("exportChatInviteLink", args).asString();
}

ChatInviteLink::Ptr ApiImpl::createChatInviteLink(
    std::variant<std::int64_t, std::string> chatId,
    optional<std::chrono::system_clock::time_point> expireDate,
    optional<std::int32_t> memberLimit, const optional<std::string_view> name,
    optional<bool> createsJoinRequest) const {
    return parse<ChatInviteLink>(sendRequest(
        "createChatInviteLink",
        build(std::pair{"chat_id", chatId},
              std::pair{"expire_date", expireDate},
              std::pair{"member_limit", memberLimit}, std::pair{"name", name},
              std::pair{"creates_join_request", createsJoinRequest})));
}

ChatInviteLink::Ptr ApiImpl::editChatInviteLink(
    std::variant<std::int64_t, std::string> chatId,
    const std::string_view inviteLink,
    optional<std::chrono::system_clock::time_point> expireDate,
    optional<std::int32_t> memberLimit, const optional<std::string_view> name,
    optional<bool> createsJoinRequest) const {
    return parse<ChatInviteLink>(sendRequest(
        "editChatInviteLink",
        build(std::pair{"chat_id", chatId},
              std::pair{"invite_link", inviteLink},
              std::pair{"expire_date", expireDate},
              std::pair{"member_limit", memberLimit}, std::pair{"name", name},
              std::pair{"creates_join_request", createsJoinRequest})));
}

ChatInviteLink::Ptr ApiImpl::revokeChatInviteLink(
    std::variant<std::int64_t, std::string> chatId,
    const std::string_view inviteLink) const {
    return parse<ChatInviteLink>(sendRequest(
        "revokeChatInviteLink", build(std::pair{"chat_id", chatId},
                                      std::pair{"invite_link", inviteLink})));
}

bool ApiImpl::approveChatJoinRequest(
    std::variant<std::int64_t, std::string> chatId, std::int64_t userId) const {
    return sendRequest("approveChatJoinRequest",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"user_id", userId}))
        .asBool();
}

bool ApiImpl::declineChatJoinRequest(
    std::variant<std::int64_t, std::string> chatId, std::int64_t userId) const {
    return sendRequest("declineChatJoinRequest",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"user_id", userId}))
        .asBool();
}

bool ApiImpl::setChatPhoto(std::variant<std::int64_t, std::string> chatId,
                           const InputFile::Ptr photo) const {
    return sendRequest("setChatPhoto", build(std::pair{"chat_id", chatId},
                                             std::pair{"photo", photo}))
        .asBool();
}

bool ApiImpl::deleteChatPhoto(
    std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("deleteChatPhoto", build(std::pair{"chat_id", chatId}))
        .asBool();
}

bool ApiImpl::setChatTitle(std::variant<std::int64_t, std::string> chatId,
                           const std::string_view title) const {
    return sendRequest("setChatTitle", build(std::pair{"chat_id", chatId},
                                             std::pair{"title", title}))
        .asBool();
}

bool ApiImpl::setChatDescription(std::variant<std::int64_t, std::string> chatId,
                                 const std::string_view description) const {
    return sendRequest("setChatDescription",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"description", description}))
        .asBool();
}

bool ApiImpl::pinChatMessage(std::variant<std::int64_t, std::string> chatId,
                             std::int32_t messageId,
                             optional<bool> disableNotification) const {
    return sendRequest(
               "pinChatMessage",
               build(std::pair{"chat_id", chatId},
                     std::pair{"message_id", messageId},
                     std::pair{"disable_notification", disableNotification}))
        .asBool();
}

bool ApiImpl::unpinChatMessage(std::variant<std::int64_t, std::string> chatId,
                               optional<std::int32_t> messageId) const {
    return sendRequest("unpinChatMessage",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_id", messageId}))
        .asBool();
}

bool ApiImpl::unpinAllChatMessages(
    std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("unpinAllChatMessages",
                       build(std::pair{"chat_id", chatId}))
        .asBool();
}

bool ApiImpl::leaveChat(std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("leaveChat", build(std::pair{"chat_id", chatId}))
        .asBool();
}

Chat::Ptr ApiImpl::getChat(
    std::variant<std::int64_t, std::string> chatId) const {
    return parse<Chat>(
        sendRequest("getChat", build(std::pair{"chat_id", chatId})));
}

std::vector<ChatMember::Ptr> ApiImpl::getChatAdministrators(
    std::variant<std::int64_t, std::string> chatId) const {
    return parseArray<ChatMember>(sendRequest(
        "getChatAdministrators", build(std::pair{"chat_id", chatId})));
}

int32_t ApiImpl::getChatMemberCount(
    std::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return sendRequest("getChatMemberCount", args).asInt();
}

ChatMember::Ptr ApiImpl::getChatMember(
    std::variant<std::int64_t, std::string> chatId, std::int64_t userId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);

    return parse<ChatMember>(sendRequest("getChatMember", args));
}

bool ApiImpl::setChatStickerSet(std::variant<std::int64_t, std::string> chatId,
                                const std::string_view stickerSetName) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("sticker_set_name	", stickerSetName);

    return sendRequest("setChatStickerSet", args).asBool();
}

bool ApiImpl::deleteChatStickerSet(
    std::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return sendRequest("deleteChatStickerSet", args).asBool();
}

std::vector<Sticker::Ptr> ApiImpl::getForumTopicIconStickers() const {
    return parseArray<Sticker>(sendRequest("getForumTopicIconStickers"));
}

ForumTopic::Ptr ApiImpl::createForumTopic(
    std::variant<std::int64_t, std::string> chatId, const std::string_view name,
    optional<std::int32_t> iconColor,
    const optional<std::string_view> iconCustomEmojiId) const {
    return parse<ForumTopic>(sendRequest(
        "createForumTopic",
        build(std::pair{"chat_id", chatId}, std::pair{"name", name},
              std::pair{"icon_color", iconColor},
              std::pair{"icon_custom_emoji_id", iconCustomEmojiId})));
}

bool ApiImpl::editForumTopic(
    std::variant<std::int64_t, std::string> chatId,
    std::int32_t messageThreadId, const optional<std::string_view> name,
    std::variant<std::int32_t, std::string> iconCustomEmojiId) const {
    return sendRequest(
               "editForumTopic",
               build(std::pair{"chat_id", chatId},
                     std::pair{"message_thread_id", messageThreadId},
                     std::pair{"name", name},
                     std::pair{"icon_custom_emoji_id", iconCustomEmojiId}))
        .asBool();
}

bool ApiImpl::closeForumTopic(std::variant<std::int64_t, std::string> chatId,
                              std::int32_t messageThreadId) const {
    return sendRequest("closeForumTopic",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_thread_id", messageThreadId}))
        .asBool();
}

bool ApiImpl::reopenForumTopic(std::variant<std::int64_t, std::string> chatId,
                               std::int32_t messageThreadId) const {
    return sendRequest("reopenForumTopic",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_thread_id", messageThreadId}))
        .asBool();
}

bool ApiImpl::deleteForumTopic(std::variant<std::int64_t, std::string> chatId,
                               std::int32_t messageThreadId) const {
    return sendRequest("deleteForumTopic",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_thread_id", messageThreadId}))
        .asBool();
}

bool ApiImpl::unpinAllForumTopicMessages(
    std::variant<std::int64_t, std::string> chatId,
    std::int32_t messageThreadId) const {
    return sendRequest("unpinAllForumTopicMessages",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_thread_id", messageThreadId}))
        .asBool();
}

bool ApiImpl::editGeneralForumTopic(
    std::variant<std::int64_t, std::string> chatId, std::string name) const {
    return sendRequest(
               "editGeneralForumTopic",
               build(std::pair{"chat_id", chatId}, std::pair{"name", name}))
        .asBool();
}

bool ApiImpl::closeGeneralForumTopic(
    std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("closeGeneralForumTopic",
                       build(std::pair{"chat_id", chatId}))
        .asBool();
}

bool ApiImpl::reopenGeneralForumTopic(
    std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("reopenGeneralForumTopic",
                       build(std::pair{"chat_id", chatId}))
        .asBool();
}

bool ApiImpl::hideGeneralForumTopic(
    std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("hideGeneralForumTopic",
                       build(std::pair{"chat_id", chatId}))
        .asBool();
}

bool ApiImpl::unhideGeneralForumTopic(
    std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("unhideGeneralForumTopic",
                       build(std::pair{"chat_id", chatId}))
        .asBool();
}

bool ApiImpl::unpinAllGeneralForumTopicMessages(
    std::variant<std::int64_t, std::string> chatId) const {
    return sendRequest("unpinAllGeneralForumTopicMessages",
                       build(std::pair{"chat_id", chatId}))
        .asBool();
}

bool ApiImpl::answerCallbackQuery(const std::string_view callbackQueryId,
                                  const optional<std::string_view> text,
                                  optional<bool> showAlert,
                                  const optional<std::string_view> url,
                                  optional<std::int32_t> cacheTime) const {
    return sendRequest(
               "answerCallbackQuery",
               build(std::pair{"callback_query_id", callbackQueryId},
                     std::pair{"text", text},
                     std::pair{"show_alert", showAlert}, std::pair{"url", url},
                     std::pair{"cache_time", cacheTime}))
        .asBool();
}

UserChatBoosts::Ptr ApiImpl::getUserChatBoosts(
    std::variant<std::int64_t, std::string> chatId, std::int32_t userId) const {
    return parse<UserChatBoosts>(sendRequest(
        "getUserChatBoosts",
        build(std::pair{"chat_id", chatId}, std::pair{"user_id", userId})));
}

BusinessConnection::Ptr ApiImpl::getBusinessConnection(
    const std::string_view businessConnectionId) const {
    return parse<BusinessConnection>(sendRequest(
        "getBusinessConnection",
        build(std::pair{"business_connection_id", businessConnectionId})));
}

bool ApiImpl::setMyCommands(const std::vector<BotCommand::Ptr> &commands,
                            BotCommandScope::Ptr scope,
                            const optional<LanguageCode> languageCode) const {
    return sendRequest(
               "setMyCommands",
               build(std::pair{"commands", commands}, std::pair{"scope", scope},
                     std::pair{"language_code", languageCode}))
        .asBool();
}

bool ApiImpl::deleteMyCommands(
    BotCommandScope::Ptr scope,
    const optional<LanguageCode> languageCode) const {
    return sendRequest("deleteMyCommands",
                       build(std::pair{"scope", scope},
                             std::pair{"language_code", languageCode}))
        .asBool();
}

std::vector<BotCommand::Ptr> ApiImpl::getMyCommands(
    BotCommandScope::Ptr scope,
    const optional<LanguageCode> languageCode) const {
    return parseArray<BotCommand>(sendRequest(
        "getMyCommands", build(std::pair{"scope", scope},
                               std::pair{"language_code", languageCode})));
}

bool ApiImpl::setMyName(const optional<std::string_view> name,
                        const optional<LanguageCode> languageCode) const {
    return sendRequest("setMyName",
                       build(std::pair{"name", name},
                             std::pair{"language_code", languageCode}))
        .asBool();
}

BotName::Ptr ApiImpl::getMyName(
    const optional<LanguageCode> languageCode) const {
    return parse<BotName>(sendRequest(
        "getMyName", build(std::pair{"language_code", languageCode})));
}

bool ApiImpl::setMyDescription(
    const optional<std::string_view> description,
    const optional<LanguageCode> languageCode) const {
    return sendRequest("setMyDescription",
                       build(std::pair{"description", description},
                             std::pair{"language_code", languageCode}))
        .asBool();
}

BotDescription::Ptr ApiImpl::getMyDescription(
    const optional<LanguageCode> languageCode) const {
    return parse<BotDescription>(sendRequest(
        "getMyDescription", build(std::pair{"language_code", languageCode})));
}

bool ApiImpl::setMyShortDescription(
    const optional<std::string_view> shortDescription,
    const optional<LanguageCode> languageCode) const {
    return sendRequest("setMyShortDescription",
                       build(std::pair{"short_description", shortDescription},
                             std::pair{"language_code", languageCode}))
        .asBool();
}

BotShortDescription::Ptr ApiImpl::getMyShortDescription(
    const optional<LanguageCode> languageCode) const {
    return parse<BotShortDescription>(
        sendRequest("getMyShortDescription",
                    build(std::pair{"language_code", languageCode})));
}

bool ApiImpl::setChatMenuButton(optional<std::int64_t> chatId,
                                MenuButton::Ptr menuButton) const {
    return sendRequest("setChatMenuButton",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"menu_button", menuButton}))
        .asBool();
}

MenuButton::Ptr ApiImpl::getChatMenuButton(
    optional<std::int64_t> chatId) const {
    return parse<MenuButton>(
        sendRequest("getChatMenuButton", build(std::pair{"chat_id", chatId})));
}

bool ApiImpl::setMyDefaultAdministratorRights(
    ChatAdministratorRights::Ptr rights, optional<bool> forChannels) const {
    return sendRequest("setMyDefaultAdministratorRights",
                       build(std::pair{"rights", rights},
                             std::pair{"for_channels", forChannels}))
        .asBool();
}

ChatAdministratorRights::Ptr ApiImpl::getMyDefaultAdministratorRights(
    optional<bool> forChannels) const {
    return parse<ChatAdministratorRights>(
        sendRequest("getMyDefaultAdministratorRights",
                    build(std::pair{"for_channels", forChannels})));
}

Message::Ptr ApiImpl::editMessageText(
    const std::string_view text, std::variant<std::int64_t, std::string> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    const optional<ParseMode> parseMode,
    LinkPreviewOptions::Ptr linkPreviewOptions,
    InlineKeyboardMarkup::Ptr replyMarkup,
    const std::vector<MessageEntity::Ptr> &entities) const {
    const auto p =
        sendRequest("editMessageText",
                    build(std::pair{"text", text}, std::pair{"chat_id", chatId},
                          std::pair{"message_id", messageId},
                          std::pair{"inline_message_id", inlineMessageId},
                          std::pair{"parse_mode", parseMode},
                          std::pair{"reply_markup", replyMarkup},
                          std::pair{"entities", entities},
                          std::pair{"link_preview", linkPreviewOptions}));
    if (p.isMember("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr ApiImpl::editMessageCaption(
    std::variant<std::int64_t, std::string> chatId,
    optional<std::int32_t> messageId, const optional<std::string_view> caption,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup, const optional<ParseMode> parseMode,
    const std::vector<MessageEntity::Ptr> &captionEntities) const {
    const auto p = sendRequest(
        "editMessageCaption",
        build(std::pair{"chat_id", chatId}, std::pair{"message_id", messageId},
              std::pair{"caption", caption},
              std::pair{"inline_message_id", inlineMessageId},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"parse_mode", parseMode},
              std::pair{"caption_entities", captionEntities}));
    if (p.isMember("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr ApiImpl::editMessageMedia(
    InputMedia::Ptr media, std::variant<std::int64_t, std::string> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup) const {
    const auto &p = sendRequest(
        "editMessageMedia",
        build(std::pair{"media", media}, std::pair{"chat_id", chatId},
              std::pair{"message_id", messageId},
              std::pair{"inline_message_id", inlineMessageId},
              std::pair{"reply_markup", replyMarkup}));
    if (p.isMember("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Message::Ptr ApiImpl::editMessageReplyMarkup(
    std::variant<std::int64_t, std::string> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId,
    GenericReply::Ptr replyMarkup) const {
    const auto &p = sendRequest(
        "editMessageReplyMarkup",
        build(std::pair{"chat_id", chatId}, std::pair{"message_id", messageId},
              std::pair{"inline_message_id", inlineMessageId},
              std::pair{"reply_markup", replyMarkup}));
    if (p.isMember("message_id")) {
        return parse<Message>(p);
    } else {
        return nullptr;
    }
}

Poll::Ptr ApiImpl::stopPoll(std::variant<std::int64_t, std::string> chatId,
                            std::int64_t messageId,
                            const InlineKeyboardMarkup::Ptr replyMarkup) const {
    return parse<Poll>(sendRequest(
        "stopPoll",
        build(std::pair{"chat_id", chatId}, std::pair{"message_id", messageId},
              std::pair{"reply_markup", replyMarkup})));
}

bool ApiImpl::deleteMessage(std::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageId) const {
    return sendRequest("deleteMessage",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_id", messageId}))
        .asBool();
}

bool ApiImpl::deleteMessages(
    std::variant<std::int64_t, std::string> chatId,
    const std::vector<std::int32_t> &messageIds) const {
    return sendRequest("deleteMessages",
                       build(std::pair{"chat_id", chatId},
                             std::pair{"message_ids", messageIds}))
        .asBool();
}

Message::Ptr ApiImpl::sendSticker(
    std::variant<std::int64_t, std::string> chatId,
    std::variant<InputFile::Ptr, std::string> sticker,
    ReplyParameters::Ptr replyParameters, GenericReply::Ptr replyMarkup,
    optional<bool> disableNotification, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent, const optional<std::string_view> emoji,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendSticker",
        build(std::pair{"chat_id", chatId}, std::pair{"sticker", sticker},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"reply_params", replyParameters},
              std::pair{"disable_notification", disableNotification},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"emoji", emoji},
              std::pair{"business_connection_id", businessConnectionId})));
}

StickerSet::Ptr ApiImpl::getStickerSet(const std::string_view name) const {
    return parse<StickerSet>(
        sendRequest("getStickerSet", build(std::pair{"name", name})));
}

std::vector<Sticker::Ptr> ApiImpl::getCustomEmojiStickers(
    const std::vector<std::string> &customEmojiIds) const {
    return parseArray<Sticker>(
        sendRequest("getCustomEmojiStickers",
                    build(std::pair{"custom_emoji_ids", customEmojiIds})));
}

File::Ptr ApiImpl::uploadStickerFile(std::int64_t userId,
                                     InputFile::Ptr sticker,
                                     const StickerFormat stickerFormat) const {
    return parse<File>(sendRequest(
        "uploadStickerFile",
        build(std::pair{"user_id", userId}, std::pair{"sticker", sticker},
              std::pair{"sticker_format", stickerFormat})));
}

bool ApiImpl::createNewStickerSet(
    std::int64_t userId, const std::string_view name,
    const std::string_view title,
    const std::vector<InputSticker::Ptr> &stickers,
    optional_default<Sticker::Type, Sticker::Type::Regular> stickerType,
    optional<bool> needsRepainting) const {
    return sendRequest(
               "createNewStickerSet",
               build(std::pair{"user_id", userId}, std::pair{"name", name},
                     std::pair{"title", title}, std::pair{"stickers", stickers},
                     std::pair{"sticker_type", stickerType},
                     std::pair{"needs_repainting", needsRepainting}))
        .asBool();
}

bool ApiImpl::addStickerToSet(std::int64_t userId, const std::string_view name,
                              InputSticker::Ptr sticker) const {
    return sendRequest("addStickerToSet", build(std::pair{"user_id", userId},
                                                std::pair{"name", name},
                                                std::pair{"sticker", sticker}))
        .asBool();
}

bool ApiImpl::setStickerPositionInSet(const std::string_view sticker,
                                      std::int32_t position) const {
    return sendRequest("setStickerPositionInSet",
                       build(std::pair{"sticker", sticker},
                             std::pair{"position", position}))
        .asBool();
}

bool ApiImpl::deleteStickerFromSet(const std::string_view sticker) const {
    return sendRequest("deleteStickerFromSet",
                       build(std::pair{"sticker", sticker}))
        .asBool();
}

bool ApiImpl::replaceStickerInSet(std::int64_t userId,
                                  const std::string_view name,
                                  const std::string_view oldSticker,
                                  InputSticker::Ptr sticker) const {
    return sendRequest(
               "replaceStickerInSet",
               build(std::pair{"user_id", userId}, std::pair{"name", name},
                     std::pair{"old_sticker", oldSticker},
                     std::pair{"sticker", sticker}))
        .asBool();
}

bool ApiImpl::setStickerEmojiList(
    const std::string_view sticker,
    const std::vector<std::string> &emojiList) const {
    return sendRequest("setStickerEmojiList",
                       build(std::pair{"sticker", sticker},
                             std::pair{"emoji_list", emojiList}))
        .asBool();
}

bool ApiImpl::setStickerKeywords(
    const std::string_view sticker,
    const std::vector<std::string> &keywords) const {
    return sendRequest("setStickerKeywords",
                       build(std::pair{"sticker", sticker},
                             std::pair{"keywords", keywords}))
        .asBool();
}

bool ApiImpl::setStickerMaskPosition(const std::string_view sticker,
                                     MaskPosition::Ptr maskPosition) const {
    return sendRequest("setStickerMaskPosition",
                       build(std::pair{"sticker", sticker},
                             std::pair{"mask_position", maskPosition}))
        .asBool();
}

bool ApiImpl::setStickerSetTitle(const std::string_view name,
                                 const std::string_view title) const {
    return sendRequest("setStickerSetTitle", build(std::pair{"name", name},
                                                   std::pair{"title", title}))
        .asBool();
}

bool ApiImpl::setStickerSetThumbnail(
    const std::string_view name, std::int64_t userId,
    const StickerFormat format,
    std::variant<InputFile::Ptr, std::string> thumbnail) const {
    return sendRequest(
               "setStickerSetThumbnail",
               build(std::pair{"name", name}, std::pair{"user_id", userId},
                     std::pair{"sticker_format", format},
                     std::pair{"thumbnail", thumbnail}))
        .asBool();
}

bool ApiImpl::setCustomEmojiStickerSetThumbnail(
    const std::string_view name,
    const optional<std::string_view> customEmojiId) const {
    return sendRequest("setCustomEmojiStickerSetThumbnail",
                       build(std::pair{"name", name},
                             std::pair{"custom_emoji_id", customEmojiId}))
        .asBool();
}

bool ApiImpl::deleteStickerSet(const std::string_view name) const {
    return sendRequest("deleteStickerSet", build(std::pair{"name", name}))
        .asBool();
}

bool ApiImpl::answerInlineQuery(
    const std::string_view inlineQueryId,
    const std::vector<InlineQueryResult::Ptr> &results,
    optional_default<std::int32_t, 300> cacheTime, optional<bool> isPersonal,
    const optional<std::string_view> nextOffset,
    InlineQueryResultsButton::Ptr button) const {
    return sendRequest("answerInlineQuery",
                       build(std::pair{"inline_query_id", inlineQueryId},
                             std::pair{"results", results},
                             std::pair{"cache_time", cacheTime},
                             std::pair{"is_personal", isPersonal},
                             std::pair{"next_offset", nextOffset},
                             std::pair{"button", button}))
        .asBool();
}

SentWebAppMessage::Ptr ApiImpl::answerWebAppQuery(
    const std::string_view webAppQueryId, InlineQueryResult::Ptr result) const {
    return parse<SentWebAppMessage>(sendRequest(
        "answerWebAppQuery", build(std::pair{"web_app_query_id", webAppQueryId},
                                   std::pair{"result", result})));
}

Message::Ptr ApiImpl::sendInvoice(
    std::variant<std::int64_t, std::string> chatId,
    const std::string_view title, const std::string_view description,
    const std::string_view payload, const std::string_view providerToken,
    const std::string_view currency,
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
        "sendInvoice",
        build(std::pair{"chat_id", chatId}, std::pair{"title", title},
              std::pair{"description", description},
              std::pair{"payload", payload},
              std::pair{"provider_token", providerToken},
              std::pair{"currency", currency}, std::pair{"prices", prices},
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
              std::pair{"reply_markup", replyMarkup},
              std::pair{"disable_notification", disableNotification},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"max_tip_amount", maxTipAmount},
              std::pair{"suggested_tip_amounts", suggestedTipAmounts},
              std::pair{"start_parameter", startParameter},
              std::pair{"protect_content", protectContent},
              std::pair{"reply_parameters", replyParameters})));
}

std::string ApiImpl::createInvoiceLink(
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
               "createInvoiceLink",
               build(std::pair{"title", title},
                     std::pair{"description", description},
                     std::pair{"payload", payload},
                     std::pair{"provider_token", providerToken},
                     std::pair{"currency", currency},
                     std::pair{"prices", prices},
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
                     std::pair{"is_flexible", isFlexible}))
        .asString();
}

bool ApiImpl::answerShippingQuery(
    const std::string_view shippingQueryId, bool ok,
    const std::vector<ShippingOption::Ptr> &shippingOptions,
    const optional<std::string_view> errorMessage) const {
    return sendRequest("answerShippingQuery",
                       build(std::pair{"shipping_query_id", shippingQueryId},
                             std::pair{"ok", ok},
                             std::pair{"shipping_options", shippingOptions},
                             std::pair{"error_message", errorMessage}))
        .asBool();
}

bool ApiImpl::answerPreCheckoutQuery(
    const std::string_view preCheckoutQueryId, bool ok,
    const optional<std::string_view> errorMessage) const {
    return sendRequest(
               "answerPreCheckoutQuery",
               build(std::pair{"pre_checkout_query_id", preCheckoutQueryId},
                     std::pair{"ok", ok},
                     std::pair{"error_message", errorMessage}))
        .asBool();
}

bool ApiImpl::setPassportDataErrors(
    std::int64_t userId,
    const std::vector<PassportElementError::Ptr> &errors) const {
    return sendRequest(
               "setPassportDataErrors",
               build(std::pair{"user_id", userId}, std::pair{"errors", errors}))
        .asBool();
}

Message::Ptr ApiImpl::sendGame(
    std::int64_t chatId, const std::string_view gameShortName,
    ReplyParameters::Ptr replyParameters, InlineKeyboardMarkup::Ptr replyMarkup,
    optional<bool> disableNotification, optional<std::int32_t> messageThreadId,
    optional<bool> protectContent,
    const optional<std::string_view> businessConnectionId) const {
    return parse<Message>(sendRequest(
        "sendGame",
        build(std::pair{"chat_id", chatId},
              std::pair{"game_short_name", gameShortName},
              std::pair{"reply_parameters", replyParameters},
              std::pair{"reply_markup", replyMarkup},
              std::pair{"disable_notification", disableNotification},
              std::pair{"message_thread_id", messageThreadId},
              std::pair{"protect_content", protectContent},
              std::pair{"business_connection_id", businessConnectionId})));
}

Message::Ptr ApiImpl::setGameScore(
    std::int64_t userId, std::int32_t score, optional<bool> force,
    optional<bool> disableEditMessage, optional<std::int64_t> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId) const {
    return parse<Message>(sendRequest(
        "setGameScore",
        build(std::pair{"user_id", userId}, std::pair{"score", score},
              std::pair{"force", force},
              std::pair{"disable_edit_message", disableEditMessage},
              std::pair{"chat_id", chatId}, std::pair{"message_id", messageId},
              std::pair{"inline_message_id", inlineMessageId})));
}

std::vector<GameHighScore::Ptr> ApiImpl::getGameHighScores(
    optional<std::int64_t> userId, optional<std::int64_t> chatId,
    optional<std::int32_t> messageId,
    const optional<std::string_view> inlineMessageId) const {
    return parseArray<GameHighScore>(sendRequest(
        "getGameHighScores",
        build(std::pair{"user_id", userId}, std::pair{"chat_id", chatId},
              std::pair{"message_id", messageId},
              std::pair{"inline_message_id", inlineMessageId})));
}

std::string ApiImpl::downloadFile(const std::string_view filePath,
                                  const std::vector<HttpReqArg> &args) const {
    std::string url(_url);
    url += "/file/bot";
    url += _token;
    url += "/";
    url += filePath;

    return _httpClient->makeRequest(url, args);
}

bool ApiImpl::blockedByUser(std::int64_t chatId) const {
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

constexpr bool kSendRequestDebug = false;

Json::Value TgBot::ApiImpl::sendRequest(
    const std::string_view method, std::vector<TgBot::HttpReqArg> args) const {
    std::string url(_url);
    url += "/bot";
    url += _token;
    url += "/";
    url += method;

    int requestRetryBackoff = _httpClient->getRequestBackoff();
    int retries = 0;
    if constexpr (kSendRequestDebug) {
        std::cout << "tgbot-cpp: Sending request: " << method << std::endl;
        for (const auto &arg : args) {
            if (arg.isFile) {
                std::cout << arg.name << "=<file:" << arg.fileName << ">"
                          << std::endl;
                continue;
            }
            std::cout << arg.name << "=" << arg.value << std::endl;
        }
    }
    while (true) {
        try {
            std::string serverResponse = _httpClient->makeRequest(url, args);

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
                std::string message =
                    "tgbot-cpp library can't parse json response.";
                throw TgException(message, TgException::ErrorCode::InvalidJson);
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
            int max_retries = _httpClient->getRequestMaxRetries();
            if ((max_retries >= 0) && (retries == max_retries)) {
                throw;
            } else {
                std::this_thread::sleep_for(
                    std::chrono::seconds(requestRetryBackoff));
                retries++;
                continue;
            }
        }
    }
}
}  // namespace TgBot
