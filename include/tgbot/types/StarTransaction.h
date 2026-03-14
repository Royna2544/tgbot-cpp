#ifndef TGBOT_STARTRANSACTION_H
#define TGBOT_STARTRANSACTION_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"

namespace TgBot {

/**
 * @brief Describes a Telegram Star transaction. Note that if the buyer initiates a chargeback with the payment provider from whom they acquired Stars (e.g., Apple, Google) following this transaction, the refunded Stars will be deducted from the bot's balance. This is outside of Telegram's control.
 *
 * @ingroup types
 */
class StarTransaction {
public:
    using Ptr = std::shared_ptr<StarTransaction>;

    /**
     * @brief Unique identifier of the transaction. Coincides with the identifier of the original transaction for refund transactions. Coincides with SuccessfulPayment.telegram_payment_charge_id for successful incoming payments from users.
     */
    std::string id;

    /**
     * @brief Integer amount of Telegram Stars transferred by the transaction
     */
    std::int64_t amount;

    /**
     * @brief Optional. The number of 1/1000000000 shares of Telegram Stars transferred by the transaction; from 0 to 999999999
     */
    std::optional<std::int64_t> nanostarAmount;

    /**
     * @brief Date the transaction was created in Unix time
     */
    std::int64_t date;

    /**
     * @brief Optional. Source of an incoming transaction (e.g., a user purchasing goods or services, Fragment refunding a failed withdrawal). Only for incoming transactions
     */
    std::optional<TransactionPartner::Ptr> source;

    /**
     * @brief Optional. Receiver of an outgoing transaction (e.g., a user for a purchase refund, Fragment for a withdrawal). Only for outgoing transactions
     */
    std::optional<TransactionPartner::Ptr> receiver;

};
}

#endif //TGBOT_STARTRANSACTION_H
