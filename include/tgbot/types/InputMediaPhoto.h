#ifndef TGBOT_INPUTMEDIAPHOTO_H
#define TGBOT_INPUTMEDIAPHOTO_H

#include "tgbot/types/InputMedia.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief Represents a photo to be sent.
 * 
 * @ingroup types
 */
class InputMediaPhoto : public InputMedia {
public:
    static constexpr std::string_view TYPE = "photo";

    using Ptr = std::shared_ptr<InputMediaPhoto>;

    InputMediaPhoto() {
        this->type = TYPE;
    }

    /**
     * @brief Optional. Pass True if the photo needs to be covered with a spoiler animation
     */
    std::optional<bool> hasSpoiler;

    /**
     * @brief Optional. Pass True, if the caption must be shown above the message media
     */
    std::optional<bool> showCaptionAboveMedia;
};
}

#endif //TGBOT_INPUTMEDIAPHOTO_H
