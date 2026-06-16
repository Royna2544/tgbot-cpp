#ifndef TGBOT_RICHBLOCKVIDEO_H
#define TGBOT_RICHBLOCKVIDEO_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichBlockCaption.h"
#include "tgbot/types/Video.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a video, corresponding to the HTML tag <video>.
 *
 * @ingroup types
 */
class RichBlockVideo : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockVideo>;

    constexpr static const char* TYPE = "video";

    RichBlockVideo() {
        type = TYPE;
    }

    /**
     * @brief The video
     */
    Video::Ptr video;

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

#endif //TGBOT_RICHBLOCKVIDEO_H
