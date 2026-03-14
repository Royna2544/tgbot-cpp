#ifndef TGBOT_ACCEPTEDGIFTTYPES_H
#define TGBOT_ACCEPTEDGIFTTYPES_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the types of gifts that can be gifted to a user or a chat.
 *
 * @ingroup types
 */
class AcceptedGiftTypes {
public:
    using Ptr = std::shared_ptr<AcceptedGiftTypes>;

    /**
     * @brief True, if unlimited regular gifts are accepted
     */
    bool unlimitedGifts;

    /**
     * @brief True, if limited regular gifts are accepted
     */
    bool limitedGifts;

    /**
     * @brief True, if unique gifts or gifts that can be upgraded to unique for free are accepted
     */
    bool uniqueGifts;

    /**
     * @brief True, if a Telegram Premium subscription is accepted
     */
    bool premiumSubscription;

    /**
     * @brief True, if transfers of unique gifts from channels are accepted
     */
    bool giftsFromChannels;

};
}

#endif //TGBOT_ACCEPTEDGIFTTYPES_H
