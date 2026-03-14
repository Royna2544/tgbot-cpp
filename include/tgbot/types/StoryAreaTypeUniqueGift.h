#ifndef TGBOT_STORYAREATYPEUNIQUEGIFT_H
#define TGBOT_STORYAREATYPEUNIQUEGIFT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/StoryAreaType.h"

namespace TgBot {

/**
 * @brief Describes a story area pointing to a unique gift. Currently, a story can have at most 1 unique gift area.
 *
 * @ingroup types
 */
class StoryAreaTypeUniqueGift : public StoryAreaType {
public:
    using Ptr = std::shared_ptr<StoryAreaTypeUniqueGift>;
    constexpr static const char* TYPE = "unique_gift";

    StoryAreaTypeUniqueGift() {
        type = TYPE;
    }

    /**
     * @brief Unique name of the gift
     */
    std::string name;

};
}

#endif //TGBOT_STORYAREATYPEUNIQUEGIFT_H
