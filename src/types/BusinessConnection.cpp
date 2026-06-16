#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BusinessConnection.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BusinessConnection> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BusinessConnection>();
    parse(data, "id", &result->id);
    result->user = parseRequired<User>(data, "user");
    parse(data, "user_chat_id", &result->userChatId);
    parse(data, "date", &result->date);
    result->rights = parse<BusinessBotRights>(data, "rights");
    parse(data, "is_enabled", &result->isEnabled);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BusinessConnection> &object) {
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
