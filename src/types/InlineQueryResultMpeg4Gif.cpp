#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultMpeg4Gif) {
    auto result = std::make_shared<InlineQueryResultMpeg4Gif>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "mpeg4_url", &result->mpeg4Url);
    parse(data, "mpeg4_width", &result->mpeg4Width);
    parse(data, "mpeg4_height", &result->mpeg4Height);
    parse(data, "mpeg4_duration", &result->mpeg4Duration);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_mime_type", &result->thumbnailMimeType);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultMpeg4Gif) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("mpeg4_url", object->mpeg4Url);
        json.put("mpeg4_width", object->mpeg4Width);
        json.put("mpeg4_height", object->mpeg4Height);
        json.put("mpeg4_duration", object->mpeg4Duration);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("thumbnail_mime_type", object->thumbnailMimeType);
        json.put("title", object->title);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("show_caption_above_media", object->showCaptionAboveMedia);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
    }
    return json;
}

} // namespace TgBot
