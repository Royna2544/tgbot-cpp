#ifndef TGBOT_RICHBLOCKANCHOR_H
#define TGBOT_RICHBLOCKANCHOR_H

#include "tgbot/types/RichBlock.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A block with an anchor, corresponding to the HTML tag <a> with the attribute name.
 *
 * @ingroup types
 */
class RichBlockAnchor : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockAnchor>;

    constexpr static const char* TYPE = "anchor";

    RichBlockAnchor() {
        type = TYPE;
    }

    /**
     * @brief The name of the anchor
     */
    std::string name;

};
}

#endif //TGBOT_RICHBLOCKANCHOR_H
