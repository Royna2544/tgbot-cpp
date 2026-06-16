#ifndef TGBOT_RICHTEXTSTRIKETHROUGH_H
#define TGBOT_RICHTEXTSTRIKETHROUGH_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A strikethrough text.
 *
 * @ingroup types
 */
class RichTextStrikethrough : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextStrikethrough>;

    constexpr static const char* TYPE = "strikethrough";

    RichTextStrikethrough() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTSTRIKETHROUGH_H
