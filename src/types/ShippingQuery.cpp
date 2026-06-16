#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ShippingQuery.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ShippingQuery> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ShippingQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
    parse(data, "invoice_payload", &result->invoicePayload);
    result->shippingAddress = parseRequired<ShippingAddress>(data, "shipping_address");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ShippingQuery> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("from", object->from);
        json.put("invoice_payload", object->invoicePayload);
        json.put("shipping_address", object->shippingAddress);
    }
    return json;
}

} // namespace TgBot
