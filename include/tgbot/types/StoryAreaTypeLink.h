#ifndef TGBOT_STORYAREATYPELINK_H
#define TGBOT_STORYAREATYPELINK_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/StoryAreaType.h"

namespace TgBot {

/**
 * @brief Describes a story area pointing to an HTTP or tg:// link. Currently, a story can have up to 3 link areas.
 *
 * @ingroup types
 */
class StoryAreaTypeLink : public StoryAreaType {
public:
    using Ptr = std::shared_ptr<StoryAreaTypeLink>;
    constexpr static const char* TYPE = "link";

    StoryAreaTypeLink() {
        type = TYPE;
    }

    /**
     * @brief HTTP or tg:// URL to be opened when the area is clicked
     */
    std::string url;

};
}

#endif //TGBOT_STORYAREATYPELINK_H
