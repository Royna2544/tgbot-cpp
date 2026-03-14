#ifndef TGBOT_STORYAREA_H
#define TGBOT_STORYAREA_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/StoryAreaPosition.h"
#include "tgbot/types/StoryAreaType.h"

namespace TgBot {

/**
 * @brief Describes a clickable area on a story media.
 *
 * @ingroup types
 */
class StoryArea {
public:
    using Ptr = std::shared_ptr<StoryArea>;

    /**
     * @brief Position of the area
     */
    StoryAreaPosition::Ptr position;

    /**
     * @brief Type of the area
     */
    StoryAreaType::Ptr type;

};
}

#endif //TGBOT_STORYAREA_H
