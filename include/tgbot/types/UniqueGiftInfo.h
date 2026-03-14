#ifndef TGBOT_UNIQUEGIFTINFO_H
#define TGBOT_UNIQUEGIFTINFO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/UniqueGift.h"

namespace TgBot {

/**
 * @brief Describes a service message about a unique gift that was sent or received.
 *
 * @ingroup types
 */
class UniqueGiftInfo {
public:
    using Ptr = std::shared_ptr<UniqueGiftInfo>;

    /**
     * @brief Information about the gift
     */
    UniqueGift::Ptr gift;

    /**
     * @brief Origin of the gift. Currently, either "upgrade" for gifts upgraded from regular gifts, "transfer" for gifts transferred from other users or channels, "resale" for gifts bought from other users, "gifted_upgrade" for upgrades purchased after the gift was sent, or "offer" for gifts bought or sold through gift purchase offers
     */
    std::string origin;

    /**
     * @brief Optional. For gifts bought from other users, the currency in which the payment for the gift was done. Currently, one of "XTR" for Telegram Stars or "TON" for toncoins.
     */
    std::optional<std::string> lastResaleCurrency;

    /**
     * @brief Optional. For gifts bought from other users, the price paid for the gift in either Telegram Stars or nanotoncoins
     */
    std::optional<std::int64_t> lastResaleAmount;

    /**
     * @brief Optional. Unique identifier of the received gift for the bot; only present for gifts received on behalf of business accounts
     */
    std::optional<std::string> ownedGiftId;

    /**
     * @brief Optional. Number of Telegram Stars that must be paid to transfer the gift; omitted if the bot cannot transfer the gift
     */
    std::optional<std::int64_t> transferStarCount;

    /**
     * @brief Optional. Point in time (Unix timestamp) when the gift can be transferred. If it is in the past, then the gift can be transferred now
     */
    std::optional<std::int64_t> nextTransferDate;

};
}

#endif //TGBOT_UNIQUEGIFTINFO_H
