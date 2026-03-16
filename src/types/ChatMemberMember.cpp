#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatMemberMember) {
    auto result = std::make_shared<ChatMemberMember>();
    parse(data, "status", &result->status);
    parse(data, "tag", &result->tag);
    result->user = parseRequired<User>(data, "user");
    parse(data, "until_date", &result->untilDate);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberMember) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        json.put("tag", object->tag);
        json.put("user", object->user);
        json.put("until_date", object->untilDate);
    }
    return json;
}

} // namespace TgBot
