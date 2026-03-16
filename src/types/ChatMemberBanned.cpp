#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatMemberBanned) {
    auto result = std::make_shared<ChatMemberBanned>();
    parse(data, "status", &result->status);
    result->user = parseRequired<User>(data, "user");
    parse(data, "until_date", &result->untilDate);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberBanned) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        json.put("user", object->user);
        json.put("until_date", object->untilDate);
    }
    return json;
}

} // namespace TgBot
