#ifndef TGBOT_RICHTEXTBANKCARDNUMBER_H
#define TGBOT_RICHTEXTBANKCARDNUMBER_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A text with a bank card number.
 *
 * @ingroup types
 */
class RichTextBankCardNumber : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextBankCardNumber>;

    constexpr static const char* TYPE = "bank_card_number";

    RichTextBankCardNumber() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The bank card number
     */
    std::string bankCardNumber;

};
}

#endif //TGBOT_RICHTEXTBANKCARDNUMBER_H
