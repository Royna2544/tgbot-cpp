#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatMemberBanned.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatMemberBanned> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatMemberBanned>();
    parse(data, "status", &result->status);
    result->user = parseRequired<User>(data, "user");
    parse(data, "until_date", &result->untilDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatMemberBanned> &object) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        json.put("user", object->user);
        json.put("until_date", object->untilDate);
    }
    return json;
}

} // namespace TgBot
