#ifndef TGBOT_BACKGROUNDTYPEPATTERN_H
#define TGBOT_BACKGROUNDTYPEPATTERN_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundType.h"
#include "tgbot/types/BackgroundFill.h"
#include "tgbot/types/Document.h"

namespace TgBot {

/**
 * @brief The background is a .PNG or .TGV (gzipped subset of SVG with MIME type "application/x-tgwallpattern") pattern to be combined with the background fill chosen by the user.
 *
 * @ingroup types
 */
class BackgroundTypePattern : public BackgroundType {
public:
    using Ptr = std::shared_ptr<BackgroundTypePattern>;
    constexpr static const char* TYPE = "pattern";

    BackgroundTypePattern() {
        type = TYPE;
    }

    /**
     * @brief Document with the pattern
     */
    Document::Ptr document;

    /**
     * @brief The background fill that is combined with the pattern
     */
    BackgroundFill::Ptr fill;

    /**
     * @brief Intensity of the pattern when it is shown above the filled background; 0-100
     */
    std::int64_t intensity;

    /**
     * @brief Optional. True, if the background fill must be applied only to the pattern itself. All other pixels are black in this case. For dark themes only
     */
    std::optional<bool> isInverted;

    /**
     * @brief Optional. True, if the background moves slightly when the device is tilted
     */
    std::optional<bool> isMoving;

};
}

#endif //TGBOT_BACKGROUNDTYPEPATTERN_H
