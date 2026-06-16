#ifndef TGBOT_RICHTEXTCODE_H
#define TGBOT_RICHTEXTCODE_H

#include "tgbot/types/RichText.h"

#include <memory>

namespace TgBot {

/**
 * @brief A monowidth text.
 *
 * @ingroup types
 */
class RichTextCode : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextCode>;

    constexpr static const char* TYPE = "code";

    RichTextCode() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

};
}

#endif //TGBOT_RICHTEXTCODE_H
