#ifndef TGBOT_INPUTRICHBLOCKANIMATION_H
#define TGBOT_INPUTRICHBLOCKANIMATION_H

#include "tgbot/types/InputMediaAnimation.h"
#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with an animation, corresponding to the HTML tag <video>.
 *
 * @ingroup types
 */
class InputRichBlockAnimation : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockAnimation>;

    constexpr static const char* TYPE = "animation";

    InputRichBlockAnimation() {
        type = TYPE;
    }

    /**
     * @brief The animation. Caption is ignored.
     */
    InputMediaAnimation::Ptr animation;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKANIMATION_H
