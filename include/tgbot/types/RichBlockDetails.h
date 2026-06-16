#ifndef TGBOT_RICHBLOCKDETAILS_H
#define TGBOT_RICHBLOCKDETAILS_H

#include "tgbot/types/RichBlock.h"
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
class RichBlockDetails : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockDetails>;

    constexpr static const char* TYPE = "details";

    RichBlockDetails() {
        type = TYPE;
    }

    /**
     * @brief Always shown summary of the block
     */
    RichText::Ptr summary;

    /**
     * @brief Content of the block
     */
    std::vector<RichBlock::Ptr> blocks;

    /**
     * @brief Optional. True, if the content of the block is visible by default
     */
    std::optional<bool> isOpen;

};
}

#endif //TGBOT_RICHBLOCKDETAILS_H
