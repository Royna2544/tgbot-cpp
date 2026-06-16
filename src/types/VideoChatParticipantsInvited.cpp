#include <tgbot/TgTypeParser.h>
#include <tgbot/types/VideoChatParticipantsInvited.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<VideoChatParticipantsInvited> parse(const nlohmann::json &data) {
    auto result = std::make_shared<VideoChatParticipantsInvited>();
    result->users = parseRequiredArray<User>(data, "users");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<VideoChatParticipantsInvited> &object) {
    JsonWrapper json;
    if (object) {
        json.put("users", object->users);
    }
    return json;
}

} // namespace TgBot
