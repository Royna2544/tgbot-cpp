#ifndef TGBOT_INPUTCONTACTMESSAGECONTENT_H
#define TGBOT_INPUTCONTACTMESSAGECONTENT_H

#include "tgbot/types/InputMessageContent.h"

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief Represents the content of a contact message to be sent as the result of an inline query.
 *
 * @ingroup types
 */
class InputContactMessageContent : public InputMessageContent {
public:
    static constexpr std::string_view TYPE = "contact";

    using Ptr = std::shared_ptr<InputContactMessageContent>;

    InputContactMessageContent() {
        this->type = TYPE;
    }

    /**
     * @brief Contact's phone number
     */
    std::string phoneNumber;

    /**
     * @brief Contact's first name
     */
    std::string firstName;

    /**
     * @brief Optional. Contact's last name
     */
    std::optional<std::string> lastName;

    /**
     * @brief Optional. Additional data about the contact in the form of a vCard, 0-2048 bytes
     */
    std::optional<std::string> vcard;
};
}

#endif //TGBOT_INPUTCONTACTMESSAGECONTENT_H
