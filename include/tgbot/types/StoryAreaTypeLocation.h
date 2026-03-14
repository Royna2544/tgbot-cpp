#ifndef TGBOT_STORYAREATYPELOCATION_H
#define TGBOT_STORYAREATYPELOCATION_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/LocationAddress.h"
#include "tgbot/types/StoryAreaType.h"

namespace TgBot {

/**
 * @brief Describes a story area pointing to a location. Currently, a story can have up to 10 location areas.
 *
 * @ingroup types
 */
class StoryAreaTypeLocation : public StoryAreaType {
public:
    using Ptr = std::shared_ptr<StoryAreaTypeLocation>;
    constexpr static const char* TYPE = "location";

    StoryAreaTypeLocation() {
        type = TYPE;
    }

    /**
     * @brief Location latitude in degrees
     */
    float latitude;

    /**
     * @brief Location longitude in degrees
     */
    float longitude;

    /**
     * @brief Optional. Address of the location
     */
    std::optional<LocationAddress::Ptr> address;

};
}

#endif //TGBOT_STORYAREATYPELOCATION_H
