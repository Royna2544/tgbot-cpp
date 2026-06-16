#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Contact.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Contact> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Contact>();
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "user_id", &result->userId);
    parse(data, "vcard", &result->vcard);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Contact> &object) {
    JsonWrapper json;
    if (object) {
        json.put("phone_number", object->phoneNumber);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("user_id", object->userId);
        json.put("vcard", object->vcard);
    }
    return json;
}

} // namespace TgBot
