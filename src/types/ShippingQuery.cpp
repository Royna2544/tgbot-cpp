#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ShippingQuery) {
    auto result = std::make_shared<ShippingQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
    parse(data, "invoice_payload", &result->invoicePayload);
    result->shippingAddress = parseRequired<ShippingAddress>(data, "shipping_address");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingQuery) {
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
