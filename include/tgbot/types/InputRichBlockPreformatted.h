#ifndef TGBOT_INPUTRICHBLOCKPREFORMATTED_H
#define TGBOT_INPUTRICHBLOCKPREFORMATTED_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief A preformatted text block, corresponding to the nested HTML tags <pre> and <code>.
 *
 * @ingroup types
 */
class InputRichBlockPreformatted : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockPreformatted>;

    constexpr static const char* TYPE = "pre";

    InputRichBlockPreformatted() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

    /**
     * @brief Optional. The programming language of the text
     */
    std::optional<std::string> language;

};
}

#endif //TGBOT_INPUTRICHBLOCKPREFORMATTED_H
