#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuccessfulPayment) {
    auto result = std::make_shared<SuccessfulPayment>();
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "subscription_expiration_date", &result->subscriptionExpirationDate);
    parse(data, "is_recurring", &result->isRecurring);
    parse(data, "is_first_recurring", &result->isFirstRecurring);
    parse(data, "shipping_option_id", &result->shippingOptionId);
    result->orderInfo = parse<OrderInfo>(data, "order_info");
    parse(data, "telegram_payment_charge_id", &result->telegramPaymentChargeId);
    parse(data, "provider_payment_charge_id", &result->providerPaymentChargeId);
    return result;
}

DECLARE_PARSER_TO_JSON(SuccessfulPayment) {
    JsonWrapper json;
    if (object) {
        json.put("currency", object->currency);
        json.put("total_amount", object->totalAmount);
        json.put("invoice_payload", object->invoicePayload);
        json.put("subscription_expiration_date", object->subscriptionExpirationDate);
        json.put("is_recurring", object->isRecurring);
        json.put("is_first_recurring", object->isFirstRecurring);
        json.put("shipping_option_id", object->shippingOptionId);
        json.put("order_info", object->orderInfo);
        json.put("telegram_payment_charge_id", object->telegramPaymentChargeId);
        json.put("provider_payment_charge_id", object->providerPaymentChargeId);
    }
    return json;
}

} // namespace TgBot
