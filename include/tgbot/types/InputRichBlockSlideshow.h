#ifndef TGBOT_INPUTRICHBLOCKSLIDESHOW_H
#define TGBOT_INPUTRICHBLOCKSLIDESHOW_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief A slideshow, corresponding to the custom HTML tag <tg-slideshow>.
 *
 * @ingroup types
 */
class InputRichBlockSlideshow : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockSlideshow>;

    constexpr static const char* TYPE = "slideshow";

    InputRichBlockSlideshow() {
        type = TYPE;
    }

    /**
     * @brief Elements of the slideshow
     */
    std::vector<InputRichBlock::Ptr> blocks;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKSLIDESHOW_H
