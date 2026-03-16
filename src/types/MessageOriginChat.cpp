#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageOriginChat) {
    auto result = std::make_shared<MessageOriginChat>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    result->senderChat = parseRequired<Chat>(data, "sender_chat");
    parse(data, "author_signature", &result->authorSignature);
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginChat) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("sender_chat", object->senderChat);
        json.put("author_signature", object->authorSignature);
    }
    return json;
}

} // namespace TgBot
