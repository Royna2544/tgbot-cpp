#ifndef TGBOT_INPUTRICHBLOCKVIDEO_H
#define TGBOT_INPUTRICHBLOCKVIDEO_H

#include "tgbot/types/InputMediaVideo.h"
#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a video, corresponding to the HTML tag <video>.
 *
 * @ingroup types
 */
class InputRichBlockVideo : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockVideo>;

    constexpr static const char* TYPE = "video";

    InputRichBlockVideo() {
        type = TYPE;
    }

    /**
     * @brief The video. Caption is ignored.
     */
    InputMediaVideo::Ptr video;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKVIDEO_H
