#ifndef TGBOT_BOTACCESSSETTINGS_H
#define TGBOT_BOTACCESSSETTINGS_H

#include "tgbot/types/User.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief This object describes the access settings of a bot.
 *
 * @ingroup types
 */
class BotAccessSettings {
public:
    using Ptr = std::shared_ptr<BotAccessSettings>;

    /**
     * @brief True, if only selected users can access the bot. The bot's owner can always access it.
     */
    bool isAccessRestricted;

    /**
     * @brief Optional. The list of other users who have access to the bot if the access is restricted
     */
    std::optional<std::vector<User::Ptr>> addedUsers;

};
}

#endif //TGBOT_BOTACCESSSETTINGS_H
