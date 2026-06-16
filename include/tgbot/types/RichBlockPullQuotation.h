#ifndef TGBOT_RICHBLOCKPULLQUOTATION_H
#define TGBOT_RICHBLOCKPULLQUOTATION_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A quotation with centered text, loosely corresponding to the HTML tag <aside>.
 *
 * @ingroup types
 */
class RichBlockPullQuotation : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockPullQuotation>;

    constexpr static const char* TYPE = "pullquote";

    RichBlockPullQuotation() {
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

#endif //TGBOT_RICHBLOCKPULLQUOTATION_H
