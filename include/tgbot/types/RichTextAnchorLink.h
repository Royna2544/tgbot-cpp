#ifndef TGBOT_RICHTEXTANCHORLINK_H
#define TGBOT_RICHTEXTANCHORLINK_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A link to an anchor.
 *
 * @ingroup types
 */
class RichTextAnchorLink : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextAnchorLink>;

    constexpr static const char* TYPE = "anchor_link";

    RichTextAnchorLink() {
        type = TYPE;
    }

    /**
     * @brief The link text
     */
    RichText::Ptr text;

    /**
     * @brief The name of the anchor. If the name is empty, then the link brings back to the top of the message.
     */
    std::string anchorName;

};
}

#endif //TGBOT_RICHTEXTANCHORLINK_H
