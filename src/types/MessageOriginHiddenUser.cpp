#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MessageOriginHiddenUser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MessageOriginHiddenUser> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MessageOriginHiddenUser>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    parse(data, "sender_user_name", &result->senderUserName);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MessageOriginHiddenUser> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("sender_user_name", object->senderUserName);
    }
    return json;
}

} // namespace TgBot
