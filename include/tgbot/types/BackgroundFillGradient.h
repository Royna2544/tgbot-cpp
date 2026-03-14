#ifndef TGBOT_BACKGROUNDFILLGRADIENT_H
#define TGBOT_BACKGROUNDFILLGRADIENT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundFill.h"

namespace TgBot {

/**
 * @brief The background is a gradient fill.
 *
 * @ingroup types
 */
class BackgroundFillGradient : public BackgroundFill {
public:
    using Ptr = std::shared_ptr<BackgroundFillGradient>;
    constexpr static const char* TYPE = "gradient";

    BackgroundFillGradient() {
        type = TYPE;
    }

    /**
     * @brief Top color of the gradient in the RGB24 format
     */
    std::int64_t topColor;

    /**
     * @brief Bottom color of the gradient in the RGB24 format
     */
    std::int64_t bottomColor;

    /**
     * @brief Clockwise rotation angle of the background fill in degrees; 0-359
     */
    std::int64_t rotationAngle;

};
}

#endif //TGBOT_BACKGROUNDFILLGRADIENT_H
