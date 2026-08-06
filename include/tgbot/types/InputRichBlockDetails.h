#ifndef TGBOT_INPUTRICHBLOCKDETAILS_H
#define TGBOT_INPUTRICHBLOCKDETAILS_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief An expandable block for details disclosure, corresponding to the HTML tag <details>.
 *
 * @ingroup types
 */
class InputRichBlockDetails : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockDetails>;

    constexpr static const char* TYPE = "details";

    InputRichBlockDetails() {
        type = TYPE;
    }

    /**
     * @brief Always shown summary of the block
     */
    RichText::Ptr summary;

    /**
     * @brief Content of the block
     */
    std::vector<InputRichBlock::Ptr> blocks;

    /**
     * @brief Optional. Pass True if the content of the block is visible by default
     */
    std::optional<bool> isOpen;

};
}

#endif //TGBOT_INPUTRICHBLOCKDETAILS_H
