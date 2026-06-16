#ifndef TGBOT_RICHBLOCKSECTIONHEADING_H
#define TGBOT_RICHBLOCKSECTIONHEADING_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichText.h"

#include <cstdint>
#include <memory>

namespace TgBot {

/**
 * @brief A section heading, corresponding to the HTML tags <h1>, <h2>, <h3>, <h4>, <h5>, or <h6>.
 *
 * @ingroup types
 */
class RichBlockSectionHeading : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockSectionHeading>;

    constexpr static const char* TYPE = "heading";

    RichBlockSectionHeading() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

    /**
     * @brief Relative size of the text font; 1-6, 1 is the largest, 6 is the smallest
     */
    std::int32_t size;

};
}

#endif //TGBOT_RICHBLOCKSECTIONHEADING_H
