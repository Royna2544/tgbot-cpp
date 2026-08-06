#ifndef TGBOT_INPUTRICHBLOCKSECTIONHEADING_H
#define TGBOT_INPUTRICHBLOCKSECTIONHEADING_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <cstdint>
#include <memory>

namespace TgBot {

/**
 * @brief A section heading, corresponding to the HTML tags <h1>, <h2>, <h3>, <h4>, <h5>, or <h6>.
 *
 * @ingroup types
 */
class InputRichBlockSectionHeading : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockSectionHeading>;

    constexpr static const char* TYPE = "heading";

    InputRichBlockSectionHeading() {
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

#endif //TGBOT_INPUTRICHBLOCKSECTIONHEADING_H
