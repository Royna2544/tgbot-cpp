#ifndef TGBOT_RICHBLOCKCOLLAGE_H
#define TGBOT_RICHBLOCKCOLLAGE_H

#include "tgbot/types/RichBlock.h"
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
class RichBlockCollage : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockCollage>;

    constexpr static const char* TYPE = "collage";

    RichBlockCollage() {
        type = TYPE;
    }

    /**
     * @brief Elements of the collage
     */
    std::vector<RichBlock::Ptr> blocks;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKCOLLAGE_H
