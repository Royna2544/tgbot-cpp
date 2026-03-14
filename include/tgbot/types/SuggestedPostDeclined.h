#ifndef TGBOT_SUGGESTEDPOSTDECLINED_H
#define TGBOT_SUGGESTEDPOSTDECLINED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

class Message;

/**
 * @brief Describes a service message about the rejection of a suggested post.
 *
 * @ingroup types
 */
class SuggestedPostDeclined {
public:
    using Ptr = std::shared_ptr<SuggestedPostDeclined>;

    /**
     * @brief Optional. Message containing the suggested post. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<std::shared_ptr<Message>> suggestedPostMessage;

    /**
     * @brief Optional. Comment with which the post was declined
     */
    std::optional<std::string> comment;

};
}

#endif //TGBOT_SUGGESTEDPOSTDECLINED_H
