#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatOwnerChanged.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatOwnerChanged> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatOwnerChanged>();
    result->newOwner = parseRequired<User>(data, "new_owner");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatOwnerChanged> &object) {
    JsonWrapper json;
    if (object) {
        json.put("new_owner", object->newOwner);
    }
    return json;
}

} // namespace TgBot
