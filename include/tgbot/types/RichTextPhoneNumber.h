#ifndef TGBOT_RICHTEXTPHONENUMBER_H
#define TGBOT_RICHTEXTPHONENUMBER_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A text with a phone number.
 *
 * @ingroup types
 */
class RichTextPhoneNumber : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextPhoneNumber>;

    constexpr static const char* TYPE = "phone_number";

    RichTextPhoneNumber() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The phone number
     */
    std::string phoneNumber;

};
}

#endif //TGBOT_RICHTEXTPHONENUMBER_H
