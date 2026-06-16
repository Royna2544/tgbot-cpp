#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MessageOriginUser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MessageOriginUser> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MessageOriginUser>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    result->senderUser = parseRequired<User>(data, "sender_user");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MessageOriginUser> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("sender_user", object->senderUser);
    }
    return json;
}

} // namespace TgBot
