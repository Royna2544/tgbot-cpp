#ifndef TGBOT_GIFTINFO_H
#define TGBOT_GIFTINFO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Gift.h"
#include "tgbot/types/MessageEntity.h"

namespace TgBot {

/**
 * @brief Describes a service message about a regular gift that was sent or received.
 *
 * @ingroup types
 */
class GiftInfo {
public:
    using Ptr = std::shared_ptr<GiftInfo>;

    /**
     * @brief Information about the gift
     */
    Gift::Ptr gift;

    /**
     * @brief Optional. Unique identifier of the received gift for the bot; only present for gifts received on behalf of business accounts
     */
    std::optional<std::string> ownedGiftId;

    /**
     * @brief Optional. Number of Telegram Stars that can be claimed by the receiver by converting the gift; omitted if conversion to Telegram Stars is impossible
     */
    std::optional<std::int64_t> convertStarCount;

    /**
     * @brief Optional. Number of Telegram Stars that were prepaid for the ability to upgrade the gift
     */
    std::optional<std::int64_t> prepaidUpgradeStarCount;

    /**
     * @brief Optional. True, if the gift's upgrade was purchased after the gift was sent
     */
    std::optional<bool> isUpgradeSeparate;

    /**
     * @brief Optional. True, if the gift can be upgraded to a unique gift
     */
    std::optional<bool> canBeUpgraded;

    /**
     * @brief Optional. Text of the message that was added to the gift
     */
    std::optional<std::string> text;

    /**
     * @brief Optional. Special entities that appear in the text
     */
    std::optional<std::vector<MessageEntity::Ptr>> entities;

    /**
     * @brief Optional. True, if the sender and gift text are shown only to the gift receiver; otherwise, everyone will be able to see them
     */
    std::optional<bool> isPrivate;

    /**
     * @brief Optional. Unique number reserved for this gift when upgraded. See the number field in UniqueGift
     */
    std::optional<std::int64_t> uniqueGiftNumber;

};
}

#endif //TGBOT_GIFTINFO_H
