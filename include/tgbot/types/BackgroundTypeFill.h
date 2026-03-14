#ifndef TGBOT_BACKGROUNDTYPEFILL_H
#define TGBOT_BACKGROUNDTYPEFILL_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundType.h"
#include "tgbot/types/BackgroundFill.h"

namespace TgBot {

/**
 * @brief The background is automatically filled based on the selected colors.
 *
 * @ingroup types
 */
class BackgroundTypeFill : public BackgroundType {
public:
    using Ptr = std::shared_ptr<BackgroundTypeFill>;

    constexpr static const char* TYPE = "fill";

    BackgroundTypeFill() {
        type = TYPE;
    }

    /**
     * @brief The background fill
     */
    BackgroundFill::Ptr fill;

    /**
     * @brief Dimming of the background in dark themes, as a percentage; 0-100
     */
    std::int64_t darkThemeDimming;

};
}

#endif //TGBOT_BACKGROUNDTYPEFILL_H
