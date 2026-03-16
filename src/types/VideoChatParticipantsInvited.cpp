#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(VideoChatParticipantsInvited) {
    auto result = std::make_shared<VideoChatParticipantsInvited>();
    result->users = parseRequiredArray<User>(data, "users");
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatParticipantsInvited) {
    JsonWrapper json;
    if (object) {
        json.put("users", object->users);
    }
    return json;
}

} // namespace TgBot
