#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatOwnerLeft.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatOwnerLeft> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatOwnerLeft>();
    result->newOwner = parse<User>(data, "new_owner");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatOwnerLeft> &object) {
    JsonWrapper json;
    if (object) {
        json.put("new_owner", object->newOwner);
    }
    return json;
}

} // namespace TgBot
