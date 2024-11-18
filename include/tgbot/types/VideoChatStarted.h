#ifndef TGBOT_VIDEOCHATSTARTED_H
#define TGBOT_VIDEOCHATSTARTED_H

#include <memory>

namespace TgBot {

/**
 * @brief This object represents a service message about a video chat started in the chat.
 * Currently holds no information.
 *
 * @ingroup types
 */
class VideoChatStarted {

public:
    using Ptr = std::shared_ptr<VideoChatStarted>;
};
}

#endif //TGBOT_VIDEOCHATSTARTED_H
