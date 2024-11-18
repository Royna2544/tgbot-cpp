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
};
}

#endif //TGBOT_GIVEAWAYCREATED_H
