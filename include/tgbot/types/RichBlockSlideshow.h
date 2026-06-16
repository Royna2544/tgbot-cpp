#ifndef TGBOT_RICHBLOCKSLIDESHOW_H
#define TGBOT_RICHBLOCKSLIDESHOW_H

#include "tgbot/types/RichBlock.h"
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
class RichBlockSlideshow : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockSlideshow>;

    constexpr static const char* TYPE = "slideshow";

    RichBlockSlideshow() {
        type = TYPE;
    }

    /**
     * @brief Elements of the slideshow
     */
    std::vector<RichBlock::Ptr> blocks;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKSLIDESHOW_H
