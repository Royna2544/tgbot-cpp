#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedPhoto) {
    auto result = std::make_shared<InlineQueryResultCachedPhoto>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "photo_file_id", &result->photoFileId);
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

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("photo_file_id", object->photoFileId);
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
