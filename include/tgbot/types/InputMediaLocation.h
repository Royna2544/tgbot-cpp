#ifndef TGBOT_INPUTMEDIALOCATION_H
#define TGBOT_INPUTMEDIALOCATION_H

#include "tgbot/types/InputMedia.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief Represents a location to be sent.
 *
 * @ingroup types
 */
class InputMediaLocation : public InputMedia {
public:
    using Ptr = std::shared_ptr<InputMediaLocation>;

    constexpr static const char* TYPE = "location";

    InputMediaLocation() {
        type = TYPE;
    }

    /**
     * @brief Latitude of the location
     */
    float latitude;

    /**
     * @brief Longitude of the location
     */
    float longitude;

    /**
     * @brief Optional. The radius of uncertainty for the location, measured in meters; 0-1500
     */
    std::optional<float> horizontalAccuracy;

};
}

#endif //TGBOT_INPUTMEDIALOCATION_H
