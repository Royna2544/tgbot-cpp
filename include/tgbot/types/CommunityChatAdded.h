#ifndef TGBOT_COMMUNITYCHATADDED_H
#define TGBOT_COMMUNITYCHATADDED_H

#include "tgbot/types/Community.h"

#include <memory>

namespace TgBot {

/**
 * @brief Describes a service message about a chat being added to a community.
 *
 * @ingroup types
 */
class CommunityChatAdded {
public:
    using Ptr = std::shared_ptr<CommunityChatAdded>;

    /**
     * @brief The new community to which the chat belongs
     */
    Community::Ptr community;

};
}

#endif //TGBOT_COMMUNITYCHATADDED_H
