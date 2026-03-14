#ifndef TGBOT_GIFT_H
#define TGBOT_GIFT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Chat.h"
#include "tgbot/types/GiftBackground.h"
#include "tgbot/types/Sticker.h"

namespace TgBot {

/**
 * @brief This object represents a gift that can be sent by the bot.
 *
 * @ingroup types
 */
class Gift {
public:
    using Ptr = std::shared_ptr<Gift>;

    /**
     * @brief Unique identifier of the gift
     */
    std::string id;

    /**
     * @brief The sticker that represents the gift
     */
    Sticker::Ptr sticker;

    /**
     * @brief The number of Telegram Stars that must be paid to send the sticker
     */
    std::int64_t starCount;

    /**
     * @brief Optional. The number of Telegram Stars that must be paid to upgrade the gift to a unique one
     */
    std::optional<std::int64_t> upgradeStarCount;

    /**
     * @brief Optional. True, if the gift can only be purchased by Telegram Premium subscribers
     */
    std::optional<bool> isPremium;

    /**
     * @brief Optional. True, if the gift can be used (after being upgraded) to customize a user's appearance
     */
    std::optional<bool> hasColors;

    /**
     * @brief Optional. The total number of gifts of this type that can be sent by all users; for limited gifts only
     */
    std::optional<std::int64_t> totalCount;

    /**
     * @brief Optional. The number of remaining gifts of this type that can be sent by all users; for limited gifts only
     */
    std::optional<std::int64_t> remainingCount;

    /**
     * @brief Optional. The total number of gifts of this type that can be sent by the bot; for limited gifts only
     */
    std::optional<std::int64_t> personalTotalCount;

    /**
     * @brief Optional. The number of remaining gifts of this type that can be sent by the bot; for limited gifts only
     */
    std::optional<std::int64_t> personalRemainingCount;

    /**
     * @brief Optional. Background of the gift
     */
    std::optional<GiftBackground::Ptr> background;

    /**
     * @brief Optional. The total number of different unique gifts that can be obtained by upgrading the gift
     */
    std::optional<std::int64_t> uniqueGiftVariantCount;

    /**
     * @brief Optional. Information about the chat that published the gift
     */
    std::optional<Chat::Ptr> publisherChat;

};
}

#endif //TGBOT_GIFT_H
