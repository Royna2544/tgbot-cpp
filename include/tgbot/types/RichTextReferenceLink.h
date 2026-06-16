#ifndef TGBOT_RICHTEXTREFERENCELINK_H
#define TGBOT_RICHTEXTREFERENCELINK_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A link to a reference.
 *
 * @ingroup types
 */
class RichTextReferenceLink : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextReferenceLink>;

    constexpr static const char* TYPE = "reference_link";

    RichTextReferenceLink() {
        type = TYPE;
    }

    /**
     * @brief The link text
     */
    RichText::Ptr text;

    /**
     * @brief The name of the reference
     */
    std::string referenceName;

};
}

#endif //TGBOT_RICHTEXTREFERENCELINK_H
