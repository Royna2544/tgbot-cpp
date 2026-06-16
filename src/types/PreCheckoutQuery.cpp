#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PreCheckoutQuery.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PreCheckoutQuery> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PreCheckoutQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "shipping_option_id", &result->shippingOptionId);
    result->orderInfo = parse<OrderInfo>(data, "order_info");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PreCheckoutQuery> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("from", object->from);
        json.put("currency", object->currency);
        json.put("total_amount", object->totalAmount);
        json.put("invoice_payload", object->invoicePayload);
        json.put("shipping_option_id", object->shippingOptionId);
        json.put("order_info", object->orderInfo);
    }
    return json;
}

} // namespace TgBot
