#ifndef TGBOT_RICHBLOCKVOICENOTE_H
#define TGBOT_RICHBLOCKVOICENOTE_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichBlockCaption.h"
#include "tgbot/types/Voice.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a voice note, corresponding to the HTML tag <audio>.
 *
 * @ingroup types
 */
class RichBlockVoiceNote : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockVoiceNote>;

    constexpr static const char* TYPE = "voice_note";

    RichBlockVoiceNote() {
        type = TYPE;
    }

    /**
     * @brief The voice note
     */
    Voice::Ptr voiceNote;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKVOICENOTE_H
