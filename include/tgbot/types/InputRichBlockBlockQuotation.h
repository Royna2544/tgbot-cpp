#ifndef TGBOT_INPUTRICHBLOCKBLOCKQUOTATION_H
#define TGBOT_INPUTRICHBLOCKBLOCKQUOTATION_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief A block quotation, corresponding to the HTML tag <blockquote>.
 *
 * @ingroup types
 */
class InputRichBlockBlockQuotation : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockBlockQuotation>;

    constexpr static const char* TYPE = "blockquote";

    InputRichBlockBlockQuotation() {
        type = TYPE;
    }

    /**
     * @brief Content of the block
     */
    std::vector<InputRichBlock::Ptr> blocks;

    /**
     * @brief Optional. Credit of the block
     */
    std::optional<RichText::Ptr> credit;

};
}

#endif //TGBOT_INPUTRICHBLOCKBLOCKQUOTATION_H
