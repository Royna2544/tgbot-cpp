#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(KeyboardButtonRequestChat) {
    auto result = std::make_shared<KeyboardButtonRequestChat>();
    parse(data, "request_id", &result->requestId);
    parse(data, "chat_is_channel", &result->chatIsChannel);
    parse(data, "chat_is_forum", &result->chatIsForum);
    parse(data, "chat_has_username", &result->chatHasUsername);
    parse(data, "chat_is_created", &result->chatIsCreated);
    result->userAdministratorRights = parse<ChatAdministratorRights>(data, "user_administrator_rights");
    result->botAdministratorRights = parse<ChatAdministratorRights>(data, "bot_administrator_rights");
    parse(data, "bot_is_member", &result->botIsMember);
    parse(data, "request_title", &result->requestTitle);
    parse(data, "request_username", &result->requestUsername);
    parse(data, "request_photo", &result->requestPhoto);
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonRequestChat) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("chat_is_channel", object->chatIsChannel);
        json.put("chat_is_forum", object->chatIsForum);
        json.put("chat_has_username", object->chatHasUsername);
        json.put("chat_is_created", object->chatIsCreated);
        json.put("user_administrator_rights", object->userAdministratorRights);
        json.put("bot_administrator_rights", object->botAdministratorRights);
        json.put("bot_is_member", object->botIsMember);
        json.put("request_title", object->requestTitle);
        json.put("request_username", object->requestUsername);
        json.put("request_photo", object->requestPhoto);
    }
    return json;
}

} // namespace TgBot
