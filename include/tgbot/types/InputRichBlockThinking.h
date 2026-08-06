#ifndef TGBOT_INPUTRICHBLOCKTHINKING_H
#define TGBOT_INPUTRICHBLOCKTHINKING_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A block with a "Thinking..." placeholder, corresponding to the custom HTML tag <tg-thinking>. The block may be used only in sendRichMessageDraft, therefore it can't be received in messages. See https://t.me/addemoji/AIActions for examples of custom emoji that are recommended for usage in the block.
 *
 * @ingroup types
 */
class InputRichBlockThinking : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockThinking>;

    constexpr static const char* TYPE = "thinking";

    InputRichBlockThinking() {
        type = TYPE;
    }

    /**
     * @brief Text of the block. See https://t.me/addemoji/AIActions for examples of custom emoji that are recommended for usage in the block.
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_INPUTRICHBLOCKTHINKING_H
