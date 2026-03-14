#ifndef TGBOT_BACKGROUNDFILL_H
#define TGBOT_BACKGROUNDFILL_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the way a background is filled based on the selected colors. Currently, it can be one of - BackgroundFillSolid - BackgroundFillGradient - BackgroundFillFreeformGradient
 *
 * @ingroup types
 */
class BackgroundFill {
public:
    using Ptr = std::shared_ptr<BackgroundFill>;

    std::string type;

};
}

#endif //TGBOT_BACKGROUNDFILL_H
