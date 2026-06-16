#include <tgbot/TgTypeParser.h>
#include <tgbot/types/OrderInfo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<OrderInfo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<OrderInfo>();
    parse(data, "name", &result->name);
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "email", &result->email);
    result->shippingAddress = parse<ShippingAddress>(data, "shipping_address");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<OrderInfo> &object) {
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
