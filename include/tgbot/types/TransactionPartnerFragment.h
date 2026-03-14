#ifndef TGBOT_TRANSACTIONPARTNERFRAGMENT_H
#define TGBOT_TRANSACTIONPARTNERFRAGMENT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"

namespace TgBot {

/**
 * @brief Describes a withdrawal transaction with Fragment.
 *
 * @ingroup types
 */
class TransactionPartnerFragment : public TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartnerFragment>;
    constexpr static const char* TYPE = "fragment";

    TransactionPartnerFragment() {
        type = TYPE;
    }

    /**
     * @brief Optional. State of the transaction if the transaction is outgoing
     */
    std::optional<RevenueWithdrawalState::Ptr> withdrawalState;

};
}

#endif //TGBOT_TRANSACTIONPARTNERFRAGMENT_H
