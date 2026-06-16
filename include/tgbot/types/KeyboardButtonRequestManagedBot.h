#ifndef TGBOT_KEYBOARDBUTTONREQUESTMANAGEDBOT_H
#define TGBOT_KEYBOARDBUTTONREQUESTMANAGEDBOT_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief This object defines the parameters for the creation of a managed bot. Information about the created bot will be shared with the bot using the update managed_bot and a Message with the field managed_bot_created.
 *
 * @ingroup types
 */
class KeyboardButtonRequestManagedBot {
public:
    using Ptr = std::shared_ptr<KeyboardButtonRequestManagedBot>;

    /**
     * @brief Signed 32-bit identifier of the request. Must be unique within the message.
     */
    std::int32_t requestId;

    /**
     * @brief Optional. Suggested name for the bot
     */
    std::optional<std::string> suggestedName;

    /**
     * @brief Optional. Suggested username for the bot
     */
    std::optional<std::string> suggestedUsername;

};
}

#endif //TGBOT_KEYBOARDBUTTONREQUESTMANAGEDBOT_H
