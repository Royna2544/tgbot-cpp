#ifndef TGBOT_STORYAREATYPE_H
#define TGBOT_STORYAREATYPE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes the type of a clickable area on a story. Currently, it can be one of - StoryAreaTypeLocation - StoryAreaTypeSuggestedReaction - StoryAreaTypeLink - StoryAreaTypeWeather - StoryAreaTypeUniqueGift
 *
 * @ingroup types
 */
class StoryAreaType {
public:
    using Ptr = std::shared_ptr<StoryAreaType>;

    std::string type;

};
}

#endif //TGBOT_STORYAREATYPE_H
