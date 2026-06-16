#ifndef TGBOT_RICHTEXTMARKED_H
#define TGBOT_RICHTEXTMARKED_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A marked text.
 *
 * @ingroup types
 */
class RichTextMarked : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextMarked>;

    constexpr static const char* TYPE = "marked";

    RichTextMarked() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTMARKED_H
