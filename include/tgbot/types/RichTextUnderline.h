#ifndef TGBOT_RICHTEXTUNDERLINE_H
#define TGBOT_RICHTEXTUNDERLINE_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief An underlined text.
 *
 * @ingroup types
 */
class RichTextUnderline : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextUnderline>;

    constexpr static const char* TYPE = "underline";

    RichTextUnderline() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTUNDERLINE_H
