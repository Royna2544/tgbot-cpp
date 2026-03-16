#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(OrderInfo) {
    auto result = std::make_shared<OrderInfo>();
    parse(data, "name", &result->name);
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "email", &result->email);
    result->shippingAddress = parse<ShippingAddress>(data, "shipping_address");
    return result;
}

DECLARE_PARSER_TO_JSON(OrderInfo) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("phone_number", object->phoneNumber);
        json.put("email", object->email);
        json.put("shipping_address", object->shippingAddress);
    }
    return json;
}

} // namespace TgBot
