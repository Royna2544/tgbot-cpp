#ifndef TGBOT_REFUNDEDPAYMENT_H
#define TGBOT_REFUNDEDPAYMENT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object contains basic information about a refunded payment.
 *
 * @ingroup types
 */
class RefundedPayment {
public:
    using Ptr = std::shared_ptr<RefundedPayment>;

    /**
     * @brief Three-letter ISO 4217 currency code, or "XTR" for payments in Telegram Stars. Currently, always "XTR"
     */
    std::string currency;

    /**
     * @brief Total refunded price in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45, total_amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).
     */
    std::int64_t totalAmount;

    /**
     * @brief Bot-specified invoice payload
     */
    std::string invoicePayload;

    /**
     * @brief Telegram payment identifier
     */
    std::string telegramPaymentChargeId;

    /**
     * @brief Optional. Provider payment identifier
     */
    std::optional<std::string> providerPaymentChargeId;

};
}

#endif //TGBOT_REFUNDEDPAYMENT_H
