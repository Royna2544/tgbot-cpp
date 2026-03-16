#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultDocument) {
    auto result = std::make_shared<InlineQueryResultDocument>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "document_url", &result->documentUrl);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "description", &result->description);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultDocument) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("title", object->title);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("document_url", object->documentUrl);
        json.put("mime_type", object->mimeType);
        json.put("description", object->description);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("thumbnail_width", object->thumbnailWidth);
        json.put("thumbnail_height", object->thumbnailHeight);
    }
    return json;
}

} // namespace TgBot
