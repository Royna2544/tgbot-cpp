#ifndef TGBOT_FORUMTOPICCLOSED_H
#define TGBOT_FORUMTOPICCLOSED_H

#include <memory>

namespace TgBot {

/**
 * @brief This object represents a service message about a forum topic closed in the chat.
 * 
 * Currently holds no information.
 *
 * @ingroup types
 */
class ForumTopicClosed {
public:
    using Ptr = std::shared_ptr<ForumTopicClosed>;
};
}

#endif //TGBOT_FORUMTOPICCLOSED_H
