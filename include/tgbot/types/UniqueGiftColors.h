#ifndef TGBOT_UNIQUEGIFTCOLORS_H
#define TGBOT_UNIQUEGIFTCOLORS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object contains information about the color scheme for a user's name, message replies and link previews based on a unique gift.
 *
 * @ingroup types
 */
class UniqueGiftColors {
public:
    using Ptr = std::shared_ptr<UniqueGiftColors>;

    /**
     * @brief Custom emoji identifier of the unique gift's model
     */
    std::string modelCustomEmojiId;

    /**
     * @brief Custom emoji identifier of the unique gift's symbol
     */
    std::string symbolCustomEmojiId;

    /**
     * @brief Main color used in light themes; RGB format
     */
    std::int64_t lightThemeMainColor;

    /**
     * @brief List of 1-3 additional colors used in light themes; RGB format
     */
    std::vector<std::int64_t> lightThemeOtherColors;

    /**
     * @brief Main color used in dark themes; RGB format
     */
    std::int64_t darkThemeMainColor;

    /**
     * @brief List of 1-3 additional colors used in dark themes; RGB format
     */
    std::vector<std::int64_t> darkThemeOtherColors;

};
}

#endif //TGBOT_UNIQUEGIFTCOLORS_H
