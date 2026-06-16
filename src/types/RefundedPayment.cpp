#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RefundedPayment.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RefundedPayment> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RefundedPayment>();
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "telegram_payment_charge_id", &result->telegramPaymentChargeId);
    parse(data, "provider_payment_charge_id", &result->providerPaymentChargeId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RefundedPayment> &object) {
    JsonWrapper json;
    if (object) {
        json.put("currency", object->currency);
        json.put("total_amount", object->totalAmount);
        json.put("invoice_payload", object->invoicePayload);
        json.put("telegram_payment_charge_id", object->telegramPaymentChargeId);
        json.put("provider_payment_charge_id", object->providerPaymentChargeId);
    }
    return json;
}

} // namespace TgBot
