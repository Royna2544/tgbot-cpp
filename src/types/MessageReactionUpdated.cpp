#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageReactionUpdated) {
    auto result = std::make_shared<MessageReactionUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    result->user = parse<User>(data, "user");
    result->actorChat = parse<Chat>(data, "actor_chat");
    parse(data, "date", &result->date);
    result->oldReaction = parseRequiredArray<ReactionType>(data, "old_reaction");
    result->newReaction = parseRequiredArray<ReactionType>(data, "new_reaction");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionUpdated) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("message_id", object->messageId);
        json.put("user", object->user);
        json.put("actor_chat", object->actorChat);
        json.put("date", object->date);
        json.put("old_reaction", object->oldReaction);
        json.put("new_reaction", object->newReaction);
    }
    return json;
}

} // namespace TgBot
