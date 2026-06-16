#ifndef TGBOT_INPUTRICHMESSAGECONTENT_H
#define TGBOT_INPUTRICHMESSAGECONTENT_H

#include "tgbot/types/InputMessageContent.h"
#include "tgbot/types/InputRichMessage.h"

#include <memory>
#include <string_view>

namespace TgBot {

/**
 * @brief Represents the content of a rich message to be sent as the result of
 * an inline query.
 *
 * @ingroup types
 */
class InputRichMessageContent : public InputMessageContent {
public:
    static constexpr std::string_view TYPE = "rich_message";

    using Ptr = std::shared_ptr<InputRichMessageContent>;

    InputRichMessageContent() {
        this->type = TYPE;
    }

    /**
     * @brief The rich message
     */
    InputRichMessage::Ptr richMessage;
};
}

#endif //TGBOT_INPUTRICHMESSAGECONTENT_H
