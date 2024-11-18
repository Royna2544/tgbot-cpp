#ifndef TGBOT_GIVEAWAYWINNERS_H
#define TGBOT_GIVEAWAYWINNERS_H

#include "tgbot/types/Chat.h"
#include "tgbot/types/User.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace TgBot {

/**
 * @brief This object represents a message about the completion of a giveaway with public winners.
 *
 * @ingroup types
 */
class GiveawayWinners {

public:
    using Ptr = std::shared_ptr<GiveawayWinners>;

    /**
     * @brief The chat that created the giveaway
     */
    Chat::Ptr chat;

    /**
     * @brief Identifier of the message with the giveaway in the chat
     */
    std::int32_t giveawayMessageId;

    /**
     * @brief Point in time (Unix timestamp) when winners of the giveaway were selected
     */
    std::uint32_t winnersSelectionDate;

    /**
     * @brief Total number of winners in the giveaway
     */
    std::int32_t winnerCount;

    /**
     * @brief List of up to 100 winners of the giveaway
     */
    std::vector<User::Ptr> winners;

    /**
     * @brief Optional. The number of other chats the user had to join in order to be eligible for the giveaway
     */
    std::optional<std::int32_t> additionalChatCount;

    /**
     * @brief Optional. The number of months the Telegram Premium subscription won from the giveaway will be active for
     */
    std::optional<std::int32_t> premiumSubscriptionMonthCount;

    /**
     * @brief Optional. Number of undistributed prizes
     */
    std::optional<std::int32_t> unclaimedPrizeCount;

    /**
     * @brief Optional. True, if only users who had joined the chats after the giveaway started were eligible to win
     */
    std::optional<bool> onlyNewMembers;

    /**
     * @brief Optional. True, if the giveaway was canceled because the payment for it was refunded
     */
    std::optional<bool> wasRefunded;

    /**
     * @brief Optional. Description of additional giveaway prize
     */
    std::optional<std::string> prizeDescription;
};
}

#endif //TGBOT_GIVEAWAYWINNERS_H
