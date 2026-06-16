#ifndef TGBOT_RICHBLOCKTHINKING_H
#define TGBOT_RICHBLOCKTHINKING_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A block with a "Thinking..." placeholder, corresponding to the custom HTML tag <tg-thinking>. The block may be used only in sendRichMessageDraft, therefore it can't be received in messages. See https://t.me/addemoji/AIActions for examples of custom emoji, which are recommended for usage in the block.
 *
 * @ingroup types
 */
class RichBlockThinking : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockThinking>;

    constexpr static const char* TYPE = "thinking";

    RichBlockThinking() {
        type = TYPE;
    }

    /**
     * @brief Text of the block. See https://t.me/addemoji/AIActions for examples of custom emoji, which are recommended for usage in the block.
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHBLOCKTHINKING_H
