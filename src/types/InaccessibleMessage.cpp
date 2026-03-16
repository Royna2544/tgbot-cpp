#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InaccessibleMessage) {
    auto result = std::make_shared<InaccessibleMessage>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    parse(data, "date", &result->date);
    return result;
}

DECLARE_PARSER_TO_JSON(InaccessibleMessage) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("message_id", object->messageId);
        json.put("date", object->date);
    }
    return json;
}

} // namespace TgBot
