#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatMemberLeft.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatMemberLeft> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatMemberLeft>();
    parse(data, "status", &result->status);
    result->user = parseRequired<User>(data, "user");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatMemberLeft> &object) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
