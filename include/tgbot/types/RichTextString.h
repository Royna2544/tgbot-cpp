#ifndef TGBOT_RICHTEXTSTRING_H
#define TGBOT_RICHTEXTSTRING_H

#include <memory>
#include <string>

#include "tgbot/types/RichText.h"

namespace TgBot {

/**
 * @brief A plain-string RichText (the "String" form of RichText).
 *
 * @ingroup types
 */
class RichTextString : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextString>;

    RichTextString() {
        kind = Kind::String;
    }

    /**
     * @brief The plain text.
     */
    std::string text;
};
}

#endif //TGBOT_RICHTEXTSTRING_H
