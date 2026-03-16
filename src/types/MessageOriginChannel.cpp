#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageOriginChannel) {
    auto result = std::make_shared<MessageOriginChannel>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    parse(data, "author_signature", &result->authorSignature);
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginChannel) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("chat", object->chat);
        json.put("message_id", object->messageId);
        json.put("author_signature", object->authorSignature);
    }
    return json;
}

} // namespace TgBot
