#ifndef TGBOT_RICHBLOCKFOOTER_H
#define TGBOT_RICHBLOCKFOOTER_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A footer, corresponding to the HTML tag <footer>.
 *
 * @ingroup types
 */
class RichBlockFooter : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockFooter>;

    constexpr static const char* TYPE = "footer";

    RichBlockFooter() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHBLOCKFOOTER_H
