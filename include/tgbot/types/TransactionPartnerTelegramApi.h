#ifndef TGBOT_TRANSACTIONPARTNERTELEGRAMAPI_H
#define TGBOT_TRANSACTIONPARTNERTELEGRAMAPI_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"

namespace TgBot {

/**
 * @brief Describes a transaction with payment for paid broadcasting.
 *
 * @ingroup types
 */
class TransactionPartnerTelegramApi : public TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartnerTelegramApi>;
    constexpr static const char* TYPE = "telegram_api";

    TransactionPartnerTelegramApi() {
        type = TYPE;
    }

    /**
     * @brief The number of successful requests that exceeded regular limits and were therefore billed
     */
    std::int64_t requestCount;

};
}

#endif //TGBOT_TRANSACTIONPARTNERTELEGRAMAPI_H
