#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UsersShared) {
    auto result = std::make_shared<UsersShared>();
    parse(data, "request_id", &result->requestId);
    result->users = parseRequiredArray<SharedUser>(data, "users");
    return result;
}

DECLARE_PARSER_TO_JSON(UsersShared) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("users", object->users);
    }
    return json;
}

} // namespace TgBot
