#ifndef TGBOT_OWNEDGIFTUNIQUE_H
#define TGBOT_OWNEDGIFTUNIQUE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/OwnedGift.h"
#include "tgbot/types/UniqueGift.h"
#include "tgbot/types/User.h"

namespace TgBot {

/**
 * @brief Describes a unique gift received and owned by a user or a chat.
 *
 * @ingroup types
 */
class OwnedGiftUnique : public OwnedGift {
public:
    using Ptr = std::shared_ptr<OwnedGiftUnique>;
    static constexpr const char* TYPE = "unique";

    OwnedGiftUnique() {
        type = TYPE;
    }

    /**
     * @brief Information about the unique gift
     */
    UniqueGift::Ptr gift;

    /**
     * @brief Optional. Unique identifier of the received gift for the bot; for gifts received on behalf of business accounts only
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
     * @brief Optional. True, if the gift is displayed on the account's profile page; for gifts received on behalf of business accounts only
     */
    std::optional<bool> isSaved;

    /**
     * @brief Optional. True, if the gift can be transferred to another owner; for gifts received on behalf of business accounts only
     */
    std::optional<bool> canBeTransferred;

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

#endif //TGBOT_OWNEDGIFTUNIQUE_H
