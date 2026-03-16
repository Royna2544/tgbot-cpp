#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultGif) {
    auto result = std::make_shared<InlineQueryResultGif>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "gif_url", &result->gifUrl);
    parse(data, "gif_width", &result->gifWidth);
    parse(data, "gif_height", &result->gifHeight);
    parse(data, "gif_duration", &result->gifDuration);
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

DECLARE_PARSER_TO_JSON(InlineQueryResultGif) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("gif_url", object->gifUrl);
        json.put("gif_width", object->gifWidth);
        json.put("gif_height", object->gifHeight);
        json.put("gif_duration", object->gifDuration);
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
