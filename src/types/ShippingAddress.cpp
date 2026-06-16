#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ShippingAddress.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ShippingAddress> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ShippingAddress>();
    parse(data, "country_code", &result->countryCode);
    parse(data, "state", &result->state);
    parse(data, "city", &result->city);
    parse(data, "street_line1", &result->streetLine1);
    parse(data, "street_line2", &result->streetLine2);
    parse(data, "post_code", &result->postCode);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ShippingAddress> &object) {
    JsonWrapper json;
    if (object) {
        json.put("country_code", object->countryCode);
        json.put("state", object->state);
        json.put("city", object->city);
        json.put("street_line1", object->streetLine1);
        json.put("street_line2", object->streetLine2);
        json.put("post_code", object->postCode);
    }
    return json;
}

} // namespace TgBot
