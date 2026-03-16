#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageOriginUser) {
    auto result = std::make_shared<MessageOriginUser>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    result->senderUser = parseRequired<User>(data, "sender_user");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginUser) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("sender_user", object->senderUser);
    }
    return json;
}

} // namespace TgBot
