#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ShippingAddress) {
    auto result = std::make_shared<ShippingAddress>();
    parse(data, "country_code", &result->countryCode);
    parse(data, "state", &result->state);
    parse(data, "city", &result->city);
    parse(data, "street_line1", &result->streetLine1);
    parse(data, "street_line2", &result->streetLine2);
    parse(data, "post_code", &result->postCode);
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingAddress) {
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
