#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatBoostUpdated) {
    auto result = std::make_shared<ChatBoostUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    result->boost = parseRequired<ChatBoost>(data, "boost");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBoostUpdated) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("boost", object->boost);
    }
    return json;
}

} // namespace TgBot
