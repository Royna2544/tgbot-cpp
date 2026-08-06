#ifndef TGBOT_INPUTRICHBLOCKFOOTER_H
#define TGBOT_INPUTRICHBLOCKFOOTER_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A footer, corresponding to the HTML tag <footer>.
 *
 * @ingroup types
 */
class InputRichBlockFooter : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockFooter>;

    constexpr static const char* TYPE = "footer";

    InputRichBlockFooter() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_INPUTRICHBLOCKFOOTER_H
