#ifndef TGBOT_RICHTEXTMENTION_H
#define TGBOT_RICHTEXTMENTION_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A mention by a username.
 *
 * @ingroup types
 */
class RichTextMention : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextMention>;

    constexpr static const char* TYPE = "mention";

    RichTextMention() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The username
     */
    std::string username;

};
}

#endif //TGBOT_RICHTEXTMENTION_H
