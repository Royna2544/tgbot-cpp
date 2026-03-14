#ifndef TGBOT_TRANSACTIONPARTNEROTHER_H
#define TGBOT_TRANSACTIONPARTNEROTHER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"

namespace TgBot {

/**
 * @brief Describes a transaction with an unknown source or recipient.
 *
 * @ingroup types
 */
class TransactionPartnerOther : public TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartnerOther>;
    constexpr static const char* TYPE = "other";

    TransactionPartnerOther() {
        type = TYPE;
    }
};
}

#endif //TGBOT_TRANSACTIONPARTNEROTHER_H
