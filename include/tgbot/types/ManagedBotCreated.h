#ifndef TGBOT_MANAGEDBOTCREATED_H
#define TGBOT_MANAGEDBOTCREATED_H

#include "tgbot/types/User.h"

#include <memory>

namespace TgBot {

/**
 * @brief This object contains information about the bot that was created to be managed by the current bot.
 *
 * @ingroup types
 */
class ManagedBotCreated {
public:
    using Ptr = std::shared_ptr<ManagedBotCreated>;

    /**
     * @brief Information about the bot. The bot's token can be fetched using the method getManagedBotToken.
     */
    User::Ptr bot;

};
}

#endif //TGBOT_MANAGEDBOTCREATED_H
