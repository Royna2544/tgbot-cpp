#ifndef TGBOT_TRANSACTIONPARTNERAFFILIATEPROGRAM_H
#define TGBOT_TRANSACTIONPARTNERAFFILIATEPROGRAM_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"

namespace TgBot {

/**
 * @brief Describes the affiliate program that issued the affiliate commission received via this transaction.
 *
 * @ingroup types
 */
class TransactionPartnerAffiliateProgram: public TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartnerAffiliateProgram>;
    constexpr static const char* TYPE = "affiliate_program";
     
    TransactionPartnerAffiliateProgram() {
        type = TYPE;
    }

    /**
     * @brief Optional. Information about the bot that sponsored the affiliate program
     */
    std::optional<User::Ptr> sponsorUser;

    /**
     * @brief The number of Telegram Stars received by the bot for each 1000 Telegram Stars received by the affiliate program sponsor from referred users
     */
    std::int64_t commissionPerMille;

};
}

#endif //TGBOT_TRANSACTIONPARTNERAFFILIATEPROGRAM_H
