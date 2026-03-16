#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultContact) {
    auto result = std::make_shared<InlineQueryResultContact>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "vcard", &result->vcard);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultContact) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("phone_number", object->phoneNumber);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("vcard", object->vcard);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("thumbnail_width", object->thumbnailWidth);
        json.put("thumbnail_height", object->thumbnailHeight);
    }
    return json;
}

} // namespace TgBot
