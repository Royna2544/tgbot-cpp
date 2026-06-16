#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MessageReactionCountUpdated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MessageReactionCountUpdated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MessageReactionCountUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    parse(data, "date", &result->date);
    result->reactions = parseRequiredArray<ReactionCount>(data, "reactions");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MessageReactionCountUpdated> &object) {
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
