#ifndef TGBOT_INLINEKEYBOARDMARKUP_H
#define TGBOT_INLINEKEYBOARDMARKUP_H

#include "tgbot/types/GenericReply.h"
#include "tgbot/types/InlineKeyboardButton.h"

#include <vector>
#include <memory>

namespace TgBot {

/**
 * @brief This object represents an [inline keyboard](https://core.telegram.org/bots/features#inline-keyboards) that appears right next to the message it belongs to.
 *
 * @ingroup types
 */
class InlineKeyboardMarkup : public GenericReply {

public:
    using Ptr = std::shared_ptr<InlineKeyboardMarkup>;

    static constexpr std::string_view TYPE = "inlinr_keyboard_markup";
    InlineKeyboardMarkup() { type = TYPE; }

    /**
     * @brief Array of button rows, each represented by an Array of InlineKeyboardButton objects
     */
    std::vector<std::vector<InlineKeyboardButton::Ptr>> inlineKeyboard;

};
}

#endif //TGBOT_INLINEKEYBOARDMARKUP_H
