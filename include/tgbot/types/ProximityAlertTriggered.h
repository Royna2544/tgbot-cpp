#ifndef TGBOT_CPP_PROXIMITYALERTTRIGGERED_H
#define TGBOT_CPP_PROXIMITYALERTTRIGGERED_H

#include "tgbot/types/User.h"

#include <memory>

namespace TgBot {

/**
 * @brief This object represents the content of a service message, sent whenever a user in the chat triggers a proximity alert set by another user.
 *
 * @ingroup types
 */
class ProximityAlertTriggered {

public:
    using Ptr = std::shared_ptr<ProximityAlertTriggered>;

    /**
     * @brief User that triggered the alert
     */
    User::Ptr traveler;

    /**
     * @brief User that set the alert
     */
    User::Ptr watcher;

    /**
     * @brief  	The distance between the users
     */
    std::int32_t distance;
};
}

#endif //TGBOT_CPP_PROXIMITYALERTTRIGGERED_H
