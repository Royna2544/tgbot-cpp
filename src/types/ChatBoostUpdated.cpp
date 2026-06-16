#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatBoostUpdated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatBoostUpdated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatBoostUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    result->boost = parseRequired<ChatBoost>(data, "boost");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatBoostUpdated> &object) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("boost", object->boost);
    }
    return json;
}

} // namespace TgBot
