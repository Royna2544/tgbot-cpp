#ifndef TGBOT_STORYAREATYPEWEATHER_H
#define TGBOT_STORYAREATYPEWEATHER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/StoryAreaType.h"

namespace TgBot {

/**
 * @brief Describes a story area containing weather information. Currently, a story can have up to 3 weather areas.
 *
 * @ingroup types
 */
class StoryAreaTypeWeather : public StoryAreaType {
public:
    using Ptr = std::shared_ptr<StoryAreaTypeWeather>;
    constexpr static const char* TYPE = "weather";

    StoryAreaTypeWeather() {
        type = TYPE;
    }

    /**
     * @brief Temperature, in degree Celsius
     */
    float temperature;

    /**
     * @brief Emoji representing the weather
     */
    std::string emoji;

    /**
     * @brief A color of the area background in the ARGB format
     */
    std::int64_t backgroundColor;

};
}

#endif //TGBOT_STORYAREATYPEWEATHER_H
