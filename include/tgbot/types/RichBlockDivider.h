#ifndef TGBOT_RICHBLOCKDIVIDER_H
#define TGBOT_RICHBLOCKDIVIDER_H

#include "tgbot/types/RichBlock.h"

#include <memory>

namespace TgBot {

/**
 * @brief A divider, corresponding to the HTML tag <hr/>.
 *
 * @ingroup types
 */
class RichBlockDivider : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockDivider>;

    constexpr static const char* TYPE = "divider";

    RichBlockDivider() {
        type = TYPE;
    }

};
}

#endif //TGBOT_RICHBLOCKDIVIDER_H
