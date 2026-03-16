#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultPhoto) {
    auto result = std::make_shared<InlineQueryResultPhoto>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "photo_url", &result->photoUrl);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "photo_width", &result->photoWidth);
    parse(data, "photo_height", &result->photoHeight);
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("photo_url", object->photoUrl);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("photo_width", object->photoWidth);
        json.put("photo_height", object->photoHeight);
        json.put("title", object->title);
        json.put("description", object->description);
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
