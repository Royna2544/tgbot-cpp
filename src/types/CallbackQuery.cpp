#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(CallbackQuery) {
    auto result = std::make_shared<CallbackQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
    result->message = parse(data["message"]);
    parse(data, "inline_message_id", &result->inlineMessageId);
    parse(data, "chat_instance", &result->chatInstance);
    parse(data, "data", &result->data);
    parse(data, "game_short_name", &result->gameShortName);
    return result;
}

DECLARE_PARSER_TO_JSON(CallbackQuery) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("from", object->from);
        json.put("message", object->message);
        json.put("inline_message_id", object->inlineMessageId);
        json.put("chat_instance", object->chatInstance);
        json.put("data", object->data);
        json.put("game_short_name", object->gameShortName);
    }
    return json;
}

} // namespace TgBot
