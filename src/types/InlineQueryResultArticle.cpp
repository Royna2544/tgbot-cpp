#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultArticle) {
    auto result = std::make_shared<InlineQueryResultArticle>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "title", &result->title);
    result->inputMessageContent = parseRequired<InputMessageContent>(data, "input_message_content");
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    parse(data, "url", &result->url);
    parse(data, "description", &result->description);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultArticle) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("title", object->title);
        json.put("input_message_content", object->inputMessageContent);
        json.put("reply_markup", object->replyMarkup);
        json.put("url", object->url);
        json.put("description", object->description);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("thumbnail_width", object->thumbnailWidth);
        json.put("thumbnail_height", object->thumbnailHeight);
    }
    return json;
}

} // namespace TgBot
