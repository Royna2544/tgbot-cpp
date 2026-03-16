#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatMemberLeft) {
    auto result = std::make_shared<ChatMemberLeft>();
    parse(data, "status", &result->status);
    result->user = parseRequired<User>(data, "user");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberLeft) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
