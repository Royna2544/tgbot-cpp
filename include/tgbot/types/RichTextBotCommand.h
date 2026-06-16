#ifndef TGBOT_RICHTEXTBOTCOMMAND_H
#define TGBOT_RICHTEXTBOTCOMMAND_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A bot command.
 *
 * @ingroup types
 */
class RichTextBotCommand : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextBotCommand>;

    constexpr static const char* TYPE = "bot_command";

    RichTextBotCommand() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The bot command
     */
    std::string botCommand;

};
}

#endif //TGBOT_RICHTEXTBOTCOMMAND_H
