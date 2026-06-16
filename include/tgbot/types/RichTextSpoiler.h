#ifndef TGBOT_RICHTEXTSPOILER_H
#define TGBOT_RICHTEXTSPOILER_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A text covered by a spoiler.
 *
 * @ingroup types
 */
class RichTextSpoiler : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextSpoiler>;

    constexpr static const char* TYPE = "spoiler";

    RichTextSpoiler() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTSPOILER_H
