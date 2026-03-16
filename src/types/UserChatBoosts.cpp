#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UserChatBoosts) {
    auto result = std::make_shared<UserChatBoosts>();
    result->boosts = parseRequiredArray<ChatBoost>(data, "boosts");
    return result;
}

DECLARE_PARSER_TO_JSON(UserChatBoosts) {
    JsonWrapper json;
    if (object) {
        json.put("boosts", object->boosts);
    }
    return json;
}

} // namespace TgBot
