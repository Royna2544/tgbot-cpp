#ifndef TGBOT_RICHBLOCKBLOCKQUOTATION_H
#define TGBOT_RICHBLOCKBLOCKQUOTATION_H

#include "tgbot/types/RichBlock.h"
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
class RichBlockBlockQuotation : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockBlockQuotation>;

    constexpr static const char* TYPE = "blockquote";

    RichBlockBlockQuotation() {
        type = TYPE;
    }

    /**
     * @brief Content of the block
     */
    std::vector<RichBlock::Ptr> blocks;

    /**
     * @brief Optional. Credit of the block
     */
    std::optional<RichText::Ptr> credit;

};
}

#endif //TGBOT_RICHBLOCKBLOCKQUOTATION_H
