#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageReactionCountUpdated) {
    auto result = std::make_shared<MessageReactionCountUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    parse(data, "date", &result->date);
    result->reactions = parseRequiredArray<ReactionCount>(data, "reactions");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionCountUpdated) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("message_id", object->messageId);
        json.put("date", object->date);
        json.put("reactions", object->reactions);
    }
    return json;
}

} // namespace TgBot
