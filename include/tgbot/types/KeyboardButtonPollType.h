#ifndef TGBOT_CPP_KEYBOARDBUTTONPOLLTYPE_H
#define TGBOT_CPP_KEYBOARDBUTTONPOLLTYPE_H

#include <optional>
#include <string>
#include <memory>

namespace TgBot {

/**
 * @brief This object represents type of a poll, which is allowed to be created and sent when the corresponding button is pressed.
 *
 * @ingroup types
 */
class KeyboardButtonPollType {

public:
    using Ptr = std::shared_ptr<KeyboardButtonPollType>;

    /**
     * @brief Optional. If quiz is passed, the user will be allowed to create only polls in the quiz mode.
     * If regular is passed, only regular polls will be allowed.
     * Otherwise, the user will be allowed to create a poll of any type.
     */
    std::optional<std::string> type;
};
}

#endif //TGBOT_CPP_KEYBOARDBUTTONPOLLTYPE_H
