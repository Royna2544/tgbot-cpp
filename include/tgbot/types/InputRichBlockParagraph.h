#ifndef TGBOT_INPUTRICHBLOCKPARAGRAPH_H
#define TGBOT_INPUTRICHBLOCKPARAGRAPH_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A text paragraph, corresponding to the HTML tag <p>.
 *
 * @ingroup types
 */
class InputRichBlockParagraph : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockParagraph>;

    constexpr static const char* TYPE = "paragraph";

    InputRichBlockParagraph() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_INPUTRICHBLOCKPARAGRAPH_H
