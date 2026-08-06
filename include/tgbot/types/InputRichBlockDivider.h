#ifndef TGBOT_INPUTRICHBLOCKDIVIDER_H
#define TGBOT_INPUTRICHBLOCKDIVIDER_H

#include "tgbot/types/InputRichBlock.h"

#include <memory>

namespace TgBot {

/**
 * @brief A divider, corresponding to the HTML tag <hr/>.
 *
 * @ingroup types
 */
class InputRichBlockDivider : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockDivider>;

    constexpr static const char* TYPE = "divider";

    InputRichBlockDivider() {
        type = TYPE;
    }

};
}

#endif //TGBOT_INPUTRICHBLOCKDIVIDER_H
