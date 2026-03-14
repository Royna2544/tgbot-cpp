#ifndef TGBOT_TRANSACTIONPARTNERTELEGRAMADS_H
#define TGBOT_TRANSACTIONPARTNERTELEGRAMADS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"

namespace TgBot {

/**
 * @brief Describes a withdrawal transaction to the Telegram Ads platform.
 *
 * @ingroup types
 */
class TransactionPartnerTelegramAds : public TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartnerTelegramAds>;
    constexpr static const char* TYPE = "telegram_ads";

    TransactionPartnerTelegramAds() {
        type = TYPE;
    }

};
}

#endif //TGBOT_TRANSACTIONPARTNERTELEGRAMADS_H
