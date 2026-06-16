#ifndef TGBOT_RICHTEXTBOLD_H
#define TGBOT_RICHTEXTBOLD_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A bold text.
 *
 * @ingroup types
 */
class RichTextBold : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextBold>;

    constexpr static const char* TYPE = "bold";

    RichTextBold() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTBOLD_H
