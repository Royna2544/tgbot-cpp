#ifndef TGBOT_AFFILIATEINFO_H
#define TGBOT_AFFILIATEINFO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Chat.h"
#include "tgbot/types/User.h"

namespace TgBot {

/**
 * @brief Contains information about the affiliate that received a commission via this transaction.
 *
 * @ingroup types
 */
class AffiliateInfo {
public:
    using Ptr = std::shared_ptr<AffiliateInfo>;

    /**
     * @brief Optional. The bot or the user that received an affiliate commission if it was received by a bot or a user
     */
    std::optional<User::Ptr> affiliateUser;

    /**
     * @brief Optional. The chat that received an affiliate commission if it was received by a chat
     */
    std::optional<Chat::Ptr> affiliateChat;

    /**
     * @brief The number of Telegram Stars received by the affiliate for each 1000 Telegram Stars received by the bot from referred users
     */
    std::int64_t commissionPerMille;

    /**
     * @brief Integer amount of Telegram Stars received by the affiliate from the transaction, rounded to 0; can be negative for refunds
     */
    std::int64_t amount;

    /**
     * @brief Optional. The number of 1/1000000000 shares of Telegram Stars received by the affiliate; from -999999999 to 999999999; can be negative for refunds
     */
    std::optional<std::int64_t> nanostarAmount;

};
}

#endif //TGBOT_AFFILIATEINFO_H
