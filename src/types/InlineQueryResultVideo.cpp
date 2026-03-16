#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultVideo) {
    auto result = std::make_shared<InlineQueryResultVideo>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "video_url", &result->videoUrl);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
    parse(data, "video_width", &result->videoWidth);
    parse(data, "video_height", &result->videoHeight);
    parse(data, "video_duration", &result->videoDuration);
    parse(data, "description", &result->description);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVideo) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("video_url", object->videoUrl);
        json.put("mime_type", object->mimeType);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("title", object->title);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("show_caption_above_media", object->showCaptionAboveMedia);
        json.put("video_width", object->videoWidth);
        json.put("video_height", object->videoHeight);
        json.put("video_duration", object->videoDuration);
        json.put("description", object->description);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
    }
    return json;
}

} // namespace TgBot
