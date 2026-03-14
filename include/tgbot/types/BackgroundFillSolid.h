#ifndef TGBOT_BACKGROUNDFILLSOLID_H
#define TGBOT_BACKGROUNDFILLSOLID_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundFill.h"

namespace TgBot {

/**
 * @brief The background is filled using the selected color.
 *
 * @ingroup types
 */
class BackgroundFillSolid : public BackgroundFill {
public:
    using Ptr = std::shared_ptr<BackgroundFillSolid>;
    constexpr static const char* TYPE = "solid";

    BackgroundFillSolid() {
        type = TYPE;
    }

    /**
     * @brief The color of the background fill in the RGB24 format
     */
    std::int64_t color;

};
}

#endif //TGBOT_BACKGROUNDFILLSOLID_H
