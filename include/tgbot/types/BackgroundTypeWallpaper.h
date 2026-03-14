#ifndef TGBOT_BACKGROUNDTYPEWALLPAPER_H
#define TGBOT_BACKGROUNDTYPEWALLPAPER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundType.h"
#include "tgbot/types/Document.h"

namespace TgBot {

/**
 * @brief The background is a wallpaper in the JPEG format.
 *
 * @ingroup types
 */
class BackgroundTypeWallpaper : public BackgroundType {
public:
    using Ptr = std::shared_ptr<BackgroundTypeWallpaper>;
    constexpr static const char* TYPE = "wallpaper";

    BackgroundTypeWallpaper() {
        type = TYPE;
    }

    /**
     * @brief Document with the wallpaper
     */
    Document::Ptr document;

    /**
     * @brief Dimming of the background in dark themes, as a percentage; 0-100
     */
    std::int64_t darkThemeDimming;

    /**
     * @brief Optional. True, if the wallpaper is downscaled to fit in a 450x450 square and then box-blurred with radius 12
     */
    std::optional<bool> isBlurred;

    /**
     * @brief Optional. True, if the background moves slightly when the device is tilted
     */
    std::optional<bool> isMoving;

};
}

#endif //TGBOT_BACKGROUNDTYPEWALLPAPER_H
