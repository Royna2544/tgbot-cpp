#ifndef TGBOT_INPUTRICHBLOCKAUDIO_H
#define TGBOT_INPUTRICHBLOCKAUDIO_H

#include "tgbot/types/InputMediaAudio.h"
#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a music file, corresponding to the HTML tag <audio>.
 *
 * @ingroup types
 */
class InputRichBlockAudio : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockAudio>;

    constexpr static const char* TYPE = "audio";

    InputRichBlockAudio() {
        type = TYPE;
    }

    /**
     * @brief The audio. Caption is ignored.
     */
    InputMediaAudio::Ptr audio;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKAUDIO_H
