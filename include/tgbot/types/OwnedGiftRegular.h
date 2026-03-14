#ifndef TGBOT_OWNEDGIFTREGULAR_H
#define TGBOT_OWNEDGIFTREGULAR_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Gift.h"
#include "tgbot/types/MessageEntity.h"
#include "tgbot/types/User.h"
#include "tgbot/types/GiftInfo.h"
#include "tgbot/types/OwnedGift.h"

namespace TgBot {

/**
 * @brief Describes a regular gift owned by a user or a chat.
 *
 * @ingroup types
 */
class OwnedGiftRegular : public OwnedGift {
public:
    using Ptr = std::shared_ptr<OwnedGiftRegular>;
    constexpr static const char* TYPE = "regular";

    OwnedGiftRegular() {
        type = TYPE;
    }

    /**
     * @brief Information about the regular gift
     */
    Gift::Ptr gift;

    /**
     * @brief Optional. Unique identifier of the gift for the bot; for gifts received on behalf of business accounts only
     */
    std::optional<std::string> ownedGiftId;

    /**
     * @brief Optional. Sender of the gift if it is a known user
     */
    std::optional<User::Ptr> senderUser;

    /**
     * @brief Date the gift was sent in Unix time
     */
    std::int64_t sendDate;

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
     * @brief Optional. True, if the gift is displayed on the account's profile page; for gifts received on behalf of business accounts only
     */
    std::optional<bool> isSaved;

    /**
     * @brief Optional. True, if the gift can be upgraded to a unique gift; for gifts received on behalf of business accounts only
     */
    std::optional<bool> canBeUpgraded;

    /**
     * @brief Optional. True, if the gift was refunded and isn't available anymore
     */
    std::optional<bool> wasRefunded;

    /**
     * @brief Optional. Number of Telegram Stars that can be claimed by the receiver instead of the gift; omitted if the gift cannot be converted to Telegram Stars; for gifts received on behalf of business accounts only
     */
    std::optional<std::int64_t> convertStarCount;

    /**
     * @brief Optional. Number of Telegram Stars that were paid for the ability to upgrade the gift
     */
    std::optional<std::int64_t> prepaidUpgradeStarCount;

    /**
     * @brief Optional. True, if the gift's upgrade was purchased after the gift was sent; for gifts received on behalf of business accounts only
     */
    std::optional<bool> isUpgradeSeparate;

    /**
     * @brief Optional. Unique number reserved for this gift when upgraded. See the number field in UniqueGift
     */
    std::optional<std::int64_t> uniqueGiftNumber;

};
}

#endif //TGBOT_OWNEDGIFTREGULAR_H
