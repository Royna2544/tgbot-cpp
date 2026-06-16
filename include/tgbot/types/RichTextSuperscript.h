#ifndef TGBOT_RICHTEXTSUPERSCRIPT_H
#define TGBOT_RICHTEXTSUPERSCRIPT_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A superscript text.
 *
 * @ingroup types
 */
class RichTextSuperscript : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextSuperscript>;

    constexpr static const char* TYPE = "superscript";

    RichTextSuperscript() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTSUPERSCRIPT_H
