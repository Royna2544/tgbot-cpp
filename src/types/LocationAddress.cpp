#include <tgbot/TgTypeParser.h>
#include <tgbot/types/LocationAddress.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<LocationAddress> parse(const nlohmann::json &data) {
    auto result = std::make_shared<LocationAddress>();
    parse(data, "country_code", &result->countryCode);
    parse(data, "state", &result->state);
    parse(data, "city", &result->city);
    parse(data, "street", &result->street);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<LocationAddress> &object) {
    JsonWrapper json;
    if (object) {
        json.put("country_code", object->countryCode);
        json.put("state", object->state);
        json.put("city", object->city);
        json.put("street", object->street);
    }
    return json;
}

} // namespace TgBot
