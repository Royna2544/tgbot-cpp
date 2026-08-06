#ifndef TGBOT_INPUTRICHBLOCKVOICENOTE_H
#define TGBOT_INPUTRICHBLOCKVOICENOTE_H

#include "tgbot/types/InputMediaVoiceNote.h"
#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a voice note, corresponding to the HTML tag <audio>.
 *
 * @ingroup types
 */
class InputRichBlockVoiceNote : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockVoiceNote>;

    constexpr static const char* TYPE = "voice_note";

    InputRichBlockVoiceNote() {
        type = TYPE;
    }

    /**
     * @brief The voice note. Caption is ignored.
     */
    InputMediaVoiceNote::Ptr voiceNote;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKVOICENOTE_H
