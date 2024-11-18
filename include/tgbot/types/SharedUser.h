#ifndef TGBOT_SHAREDUSER_H
#define TGBOT_SHAREDUSER_H

#include "tgbot/types/PhotoSize.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace TgBot {

/**
 * @brief This object contains information about a user that was shared with the bot using a KeyboardButtonRequestUser button.
 *
 * @ingroup types
 */
class SharedUser {

public:
    using Ptr = std::shared_ptr<SharedUser>;

    /**
     * @brief Identifier of the shared user.
     *
     * This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it.
     * But it has at most 52 significant bits, so 64-bit integers or double-precision float types are safe for storing these identifiers.
     * The bot may not have access to the user and could be unable to use this identifier, unless the user is already known to the bot by some other means.
     */
    std::int64_t userId;

    /**
     * @brief Optional. First name of the user, if the name was requested by the bot
     */
    std::optional<std::string> firstName;

    /**
     * @brief Optional. Last name of the user, if the name was requested by the bot
     */
    std::optional<std::string> lastName;

    /**
     * @brief Optional. Username of the user, if the username was requested by the bot
     */
    std::optional<std::string> username;

    /**
     * @brief Optional. Available sizes of the chat photo, if the photo was requested by the bot
     */
    std::vector<PhotoSize::Ptr> photo;
};
}

#endif //TGBOT_SHAREDUSER_H
