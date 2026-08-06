#ifndef TGBOT_COMMUNITYCHATREMOVED_H
#define TGBOT_COMMUNITYCHATREMOVED_H

#include <memory>

namespace TgBot {

/**
 * @brief Describes a service message about a chat being removed from a community. Currently holds no information.
 *
 * @ingroup types
 */
class CommunityChatRemoved {
public:
    using Ptr = std::shared_ptr<CommunityChatRemoved>;

};
}

#endif //TGBOT_COMMUNITYCHATREMOVED_H
