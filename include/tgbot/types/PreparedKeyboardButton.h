#ifndef TGBOT_PREPAREDKEYBOARDBUTTON_H
#define TGBOT_PREPAREDKEYBOARDBUTTON_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Describes a keyboard button to be used by a user of a Mini App.
 *
 * @ingroup types
 */
class PreparedKeyboardButton {
public:
    using Ptr = std::shared_ptr<PreparedKeyboardButton>;

    /**
     * @brief Unique identifier of the keyboard button
     */
    std::string id;

};
}

#endif //TGBOT_PREPAREDKEYBOARDBUTTON_H
