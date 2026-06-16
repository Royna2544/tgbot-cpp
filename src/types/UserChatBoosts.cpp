#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UserChatBoosts.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UserChatBoosts> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UserChatBoosts>();
    result->boosts = parseRequiredArray<ChatBoost>(data, "boosts");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UserChatBoosts> &object) {
    JsonWrapper json;
    if (object) {
        json.put("boosts", object->boosts);
    }
    return json;
}

} // namespace TgBot
