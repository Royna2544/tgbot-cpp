#ifndef TGBOT_RICHTEXTREFERENCE_H
#define TGBOT_RICHTEXTREFERENCE_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A reference.
 *
 * @ingroup types
 */
class RichTextReference : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextReference>;

    constexpr static const char* TYPE = "reference";

    RichTextReference() {
        type = TYPE;
    }

    /**
     * @brief Text of the reference
     */
    RichText::Ptr text;

    /**
     * @brief The name of the reference
     */
    std::string name;

};
}

#endif //TGBOT_RICHTEXTREFERENCE_H
