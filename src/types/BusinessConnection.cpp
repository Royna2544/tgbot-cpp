#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BusinessConnection) {
    auto result = std::make_shared<BusinessConnection>();
    parse(data, "id", &result->id);
    result->user = parseRequired<User>(data, "user");
    parse(data, "user_chat_id", &result->userChatId);
    parse(data, "date", &result->date);
    result->rights = parse<BusinessBotRights>(data, "rights");
    parse(data, "is_enabled", &result->isEnabled);
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessConnection) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("user", object->user);
        json.put("user_chat_id", object->userChatId);
        json.put("date", object->date);
        json.put("rights", object->rights);
        json.put("is_enabled", object->isEnabled);
    }
    return json;
}

} // namespace TgBot
