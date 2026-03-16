#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageOriginHiddenUser) {
    auto result = std::make_shared<MessageOriginHiddenUser>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    parse(data, "sender_user_name", &result->senderUserName);
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginHiddenUser) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("sender_user_name", object->senderUserName);
    }
    return json;
}

} // namespace TgBot
