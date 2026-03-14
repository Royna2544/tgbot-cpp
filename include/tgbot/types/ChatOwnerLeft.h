#ifndef TGBOT_CHATOWNERLEFT_H
#define TGBOT_CHATOWNERLEFT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/User.h"

namespace TgBot {

/**
 * @brief Describes a service message about the chat owner leaving the chat.
 *
 * @ingroup types
 */
class ChatOwnerLeft {
public:
    using Ptr = std::shared_ptr<ChatOwnerLeft>;

    /**
     * @brief Optional. The user which will be the new owner of the chat if the previous owner does not return to the chat
     */
    std::optional<User::Ptr> newOwner;

};
}

#endif //TGBOT_CHATOWNERLEFT_H
