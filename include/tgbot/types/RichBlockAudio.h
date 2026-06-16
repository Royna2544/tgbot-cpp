#ifndef TGBOT_RICHBLOCKAUDIO_H
#define TGBOT_RICHBLOCKAUDIO_H

#include "tgbot/types/Audio.h"
#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a music file, corresponding to the HTML tag <audio>.
 *
 * @ingroup types
 */
class RichBlockAudio : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockAudio>;

    constexpr static const char* TYPE = "audio";

    RichBlockAudio() {
        type = TYPE;
    }

    /**
     * @brief The audio
     */
    Audio::Ptr audio;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKAUDIO_H
