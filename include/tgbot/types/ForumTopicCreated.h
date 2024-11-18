#ifndef TGBOT_FORUMTOPICCREATED_H
#define TGBOT_FORUMTOPICCREATED_H

#include <cstdint>
#include <optional>
#include <string>
#include <memory>

namespace TgBot {

/**
 * @brief This object represents a service message about a new forum topic created in the chat.
 *
 * @ingroup types
 */
class ForumTopicCreated {
public:
    using Ptr = std::shared_ptr<ForumTopicCreated>;

    /**
     * @brief Name of the topic
     */
    std::string name;

    /**
     * @brief Color of the topic icon in RGB format
     */
    std::int32_t iconColor;

    /**
     * @brief Optional. Unique identifier of the custom emoji shown as the topic icon
     */
    std::optional<std::string> iconCustomEmojiId;
};
}

#endif //TGBOT_FORUMTOPICCREATED_H
