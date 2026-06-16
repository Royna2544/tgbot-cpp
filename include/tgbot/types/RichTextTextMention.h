#ifndef TGBOT_RICHTEXTTEXTMENTION_H
#define TGBOT_RICHTEXTTEXTMENTION_H

#include "tgbot/types/RichText.h"
#include "tgbot/types/User.h"

#include <memory>

namespace TgBot {

/**
 * @brief A mention of a Telegram user by their identifier.
 *
 * @ingroup types
 */
class RichTextTextMention : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextTextMention>;

    constexpr static const char* TYPE = "text_mention";

    RichTextTextMention() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The mentioned user
     */
    User::Ptr user;

};
}

#endif //TGBOT_RICHTEXTTEXTMENTION_H
