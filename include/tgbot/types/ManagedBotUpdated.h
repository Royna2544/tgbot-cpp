#ifndef TGBOT_MANAGEDBOTUPDATED_H
#define TGBOT_MANAGEDBOTUPDATED_H

#include "tgbot/types/User.h"

#include <memory>

namespace TgBot {

/**
 * @brief This object contains information about the creation, token update, or owner update of a bot that is managed by the current bot.
 *
 * @ingroup types
 */
class ManagedBotUpdated {
public:
    using Ptr = std::shared_ptr<ManagedBotUpdated>;

    /**
     * @brief User that created the bot
     */
    User::Ptr user;

    /**
     * @brief Information about the bot. Token of the bot can be fetched using the method getManagedBotToken.
     */
    User::Ptr bot;

};
}

#endif //TGBOT_MANAGEDBOTUPDATED_H
