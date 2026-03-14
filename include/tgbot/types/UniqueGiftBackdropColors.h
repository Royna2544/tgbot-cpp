#ifndef TGBOT_UNIQUEGIFTBACKDROPCOLORS_H
#define TGBOT_UNIQUEGIFTBACKDROPCOLORS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the colors of the backdrop of a unique gift.
 *
 * @ingroup types
 */
class UniqueGiftBackdropColors {
public:
    using Ptr = std::shared_ptr<UniqueGiftBackdropColors>;

    /**
     * @brief The color in the center of the backdrop in RGB format
     */
    std::int64_t centerColor;

    /**
     * @brief The color on the edges of the backdrop in RGB format
     */
    std::int64_t edgeColor;

    /**
     * @brief The color to be applied to the symbol in RGB format
     */
    std::int64_t symbolColor;

    /**
     * @brief The color for the text on the backdrop in RGB format
     */
    std::int64_t textColor;

};
}

#endif //TGBOT_UNIQUEGIFTBACKDROPCOLORS_H
