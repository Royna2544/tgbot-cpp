#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatBoostSourceGiftCode.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatBoostSourceGiftCode> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatBoostSourceGiftCode>();
    parse(data, "source", &result->source);
    result->user = parseRequired<User>(data, "user");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatBoostSourceGiftCode> &object) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
