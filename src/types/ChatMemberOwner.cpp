#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatMemberOwner) {
    auto result = std::make_shared<ChatMemberOwner>();
    parse(data, "status", &result->status);
    result->user = parseRequired<User>(data, "user");
    parse(data, "is_anonymous", &result->isAnonymous);
    parse(data, "custom_title", &result->customTitle);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberOwner) {
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
