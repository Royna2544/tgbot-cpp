#ifndef TGBOT_INPUTRICHBLOCKPULLQUOTATION_H
#define TGBOT_INPUTRICHBLOCKPULLQUOTATION_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A quotation with centered text, loosely corresponding to the HTML tag <aside>.
 *
 * @ingroup types
 */
class InputRichBlockPullQuotation : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockPullQuotation>;

    constexpr static const char* TYPE = "pullquote";

    InputRichBlockPullQuotation() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

    /**
     * @brief Optional. Credit of the block
     */
    std::optional<RichText::Ptr> credit;

};
}

#endif //TGBOT_INPUTRICHBLOCKPULLQUOTATION_H
