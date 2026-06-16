#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatMemberOwner.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatMemberOwner> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatMemberOwner>();
    parse(data, "status", &result->status);
    result->user = parseRequired<User>(data, "user");
    parse(data, "is_anonymous", &result->isAnonymous);
    parse(data, "custom_title", &result->customTitle);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatMemberOwner> &object) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        json.put("user", object->user);
        json.put("is_anonymous", object->isAnonymous);
        json.put("custom_title", object->customTitle);
    }
    return json;
}

} // namespace TgBot
