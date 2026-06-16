#ifndef TGBOT_RICHBLOCKANIMATION_H
#define TGBOT_RICHBLOCKANIMATION_H

#include "tgbot/types/Animation.h"
#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with an animation, corresponding to the HTML tag <video>.
 *
 * @ingroup types
 */
class RichBlockAnimation : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockAnimation>;

    constexpr static const char* TYPE = "animation";

    RichBlockAnimation() {
        type = TYPE;
    }

    /**
     * @brief The animation
     */
    Animation::Ptr animation;

    /**
     * @brief Optional. True, if the media preview is covered by a spoiler animation
     */
    std::optional<bool> hasSpoiler;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKANIMATION_H
