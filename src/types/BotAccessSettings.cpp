#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotAccessSettings.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotAccessSettings) {
    auto result = std::make_shared<BotAccessSettings>();
    parse(data, "is_access_restricted", &result->isAccessRestricted);
    result->addedUsers = parseArray<User>(data, "added_users");
    return result;
}

DECLARE_PARSER_TO_JSON(BotAccessSettings) {
    JsonWrapper json;
    if (object) {
        json.put("is_access_restricted", object->isAccessRestricted);
        json.put("added_users", object->addedUsers);
    }
    return json;
}

} // namespace TgBot
