#ifndef TGBOT_INPUTRICHBLOCKCOLLAGE_H
#define TGBOT_INPUTRICHBLOCKCOLLAGE_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief A collage, corresponding to the custom HTML tag <tg-collage>.
 *
 * @ingroup types
 */
class InputRichBlockCollage : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockCollage>;

    constexpr static const char* TYPE = "collage";

    InputRichBlockCollage() {
        type = TYPE;
    }

    /**
     * @brief Elements of the collage
     */
    std::vector<InputRichBlock::Ptr> blocks;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKCOLLAGE_H
