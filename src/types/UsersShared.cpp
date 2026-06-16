#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UsersShared.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UsersShared> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UsersShared>();
    parse(data, "request_id", &result->requestId);
    result->users = parseRequiredArray<SharedUser>(data, "users");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UsersShared> &object) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("users", object->users);
    }
    return json;
}

} // namespace TgBot
