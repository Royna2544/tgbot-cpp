#ifndef TGBOT_RICHTEXTSUBSCRIPT_H
#define TGBOT_RICHTEXTSUBSCRIPT_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A subscript text.
 *
 * @ingroup types
 */
class RichTextSubscript : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextSubscript>;

    constexpr static const char* TYPE = "subscript";

    RichTextSubscript() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTSUBSCRIPT_H
