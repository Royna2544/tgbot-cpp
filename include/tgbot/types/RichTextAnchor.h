#ifndef TGBOT_RICHTEXTANCHOR_H
#define TGBOT_RICHTEXTANCHOR_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief An anchor.
 *
 * @ingroup types
 */
class RichTextAnchor : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextAnchor>;

    constexpr static const char* TYPE = "anchor";

    RichTextAnchor() {
        type = TYPE;
    }

    /**
     * @brief The name of the anchor
     */
    std::string name;

};
}

#endif //TGBOT_RICHTEXTANCHOR_H
