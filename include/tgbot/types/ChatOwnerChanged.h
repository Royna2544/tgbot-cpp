#ifndef TGBOT_CHATOWNERCHANGED_H
#define TGBOT_CHATOWNERCHANGED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/User.h"

namespace TgBot {

/**
 * @brief Describes a service message about an ownership change in the chat.
 *
 * @ingroup types
 */
class ChatOwnerChanged {
public:
    using Ptr = std::shared_ptr<ChatOwnerChanged>;

    /**
     * @brief The new owner of the chat
     */
    User::Ptr newOwner;

};
}

#endif //TGBOT_CHATOWNERCHANGED_H
