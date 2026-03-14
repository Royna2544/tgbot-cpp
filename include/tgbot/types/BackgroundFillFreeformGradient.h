#ifndef TGBOT_BACKGROUNDFILLFREEFORMGRADIENT_H
#define TGBOT_BACKGROUNDFILLFREEFORMGRADIENT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundFill.h"

namespace TgBot {

/**
 * @brief The background is a freeform gradient that rotates after every message in the chat.
 *
 * @ingroup types
 */
class BackgroundFillFreeformGradient : public BackgroundFill {
public:
    using Ptr = std::shared_ptr<BackgroundFillFreeformGradient>;

    constexpr static const char* TYPE = "freeform_gradient";

    BackgroundFillFreeformGradient() {
        type = TYPE;
    }

    /**
     * @brief A list of the 3 or 4 base colors that are used to generate the freeform gradient in the RGB24 format
     */
    std::vector<std::int64_t> colors;

};
}

#endif //TGBOT_BACKGROUNDFILLFREEFORMGRADIENT_H
