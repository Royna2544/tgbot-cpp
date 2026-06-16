#ifndef TGBOT_RICHBLOCKCAPTION_H
#define TGBOT_RICHBLOCKCAPTION_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief Caption of a rich formatted block.
 *
 * @ingroup types
 */
class RichBlockCaption {
public:
    using Ptr = std::shared_ptr<RichBlockCaption>;

    /**
     * @brief Block caption
     */
    RichText::Ptr text;

    /**
     * @brief Optional. Block credit which corresponds to the HTML tag <cite>
     */
    std::optional<RichText::Ptr> credit;

};
}

#endif //TGBOT_RICHBLOCKCAPTION_H
