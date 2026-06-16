#ifndef TGBOT_RICHTEXTITALIC_H
#define TGBOT_RICHTEXTITALIC_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief An italicized text.
 *
 * @ingroup types
 */
class RichTextItalic : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextItalic>;

    constexpr static const char* TYPE = "italic";

    RichTextItalic() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTITALIC_H
