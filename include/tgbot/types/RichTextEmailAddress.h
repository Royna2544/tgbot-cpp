#ifndef TGBOT_RICHTEXTEMAILADDRESS_H
#define TGBOT_RICHTEXTEMAILADDRESS_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A text with an email address.
 *
 * @ingroup types
 */
class RichTextEmailAddress : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextEmailAddress>;

    constexpr static const char* TYPE = "email_address";

    RichTextEmailAddress() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The email address
     */
    std::string emailAddress;

};
}

#endif //TGBOT_RICHTEXTEMAILADDRESS_H
