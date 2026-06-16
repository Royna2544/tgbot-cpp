#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SharedUser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SharedUser> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SharedUser>();
    parse(data, "user_id", &result->userId);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "username", &result->username);
    result->photo = parseArray<PhotoSize>(data, "photo");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SharedUser> &object) {
    JsonWrapper json;
    if (object) {
        json.put("user_id", object->userId);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("username", object->username);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
