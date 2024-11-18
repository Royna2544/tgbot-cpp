#ifndef TGBOT_FORUMTOPICEDITED_H
#define TGBOT_FORUMTOPICEDITED_H

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief This object represents a service message about an edited forum topic.
 *
 * @ingroup types
 */
class ForumTopicEdited {
public:
    using Ptr = std::shared_ptr<ForumTopicEdited>;

    /**
     * @brief Optional. New name of the topic, if it was edited
     */
    std::optional<std::string> name;

    /**
     * @brief Optional. New identifier of the custom emoji shown as the topic icon, if it was edited; an empty string if the icon was removed
    */
    std::optional<std::string> iconCustomEmojiId;
};
}

#endif //TGBOT_FORUMTOPICEDITED_H
