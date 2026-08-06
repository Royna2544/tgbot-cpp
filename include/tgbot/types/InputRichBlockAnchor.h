#ifndef TGBOT_INPUTRICHBLOCKANCHOR_H
#define TGBOT_INPUTRICHBLOCKANCHOR_H

#include "tgbot/types/InputRichBlock.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A block with an anchor, corresponding to the HTML tag <a> with the attribute name.
 *
 * @ingroup types
 */
class InputRichBlockAnchor : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockAnchor>;

    constexpr static const char* TYPE = "anchor";

    InputRichBlockAnchor() {
        type = TYPE;
    }

    /**
     * @brief The name of the anchor
     */
    std::string name;

};
}

#endif //TGBOT_INPUTRICHBLOCKANCHOR_H
