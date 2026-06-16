#ifndef TGBOT_RICHBLOCKPARAGRAPH_H
#define TGBOT_RICHBLOCKPARAGRAPH_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A text paragraph, corresponding to the HTML tag <p>.
 *
 * @ingroup types
 */
class RichBlockParagraph : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockParagraph>;

    constexpr static const char* TYPE = "paragraph";

    RichBlockParagraph() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHBLOCKPARAGRAPH_H
