#ifndef TGBOT_GIVEAWAYCREATED_H
#define TGBOT_GIVEAWAYCREATED_H

#include <memory>

namespace TgBot {

/**
 * @brief This object represents a service message about the creation of a scheduled giveaway.
 *
 * Currently holds no information.
 *
 * @ingroup types
 */
class GiveawayCreated {

public:
    using Ptr = std::shared_ptr<GiveawayCreated>;

    /**
     * @brief Optional. The number of Telegram Stars to be split between giveaway winners; for Telegram Star giveaways only
     */
    std::optional<std::int32_t> prizeStarCount;
};
}

#endif //TGBOT_GIVEAWAYCREATED_H
