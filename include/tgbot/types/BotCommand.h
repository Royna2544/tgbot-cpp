#ifndef TGBOT_BOTCOMMAND_H
#define TGBOT_BOTCOMMAND_H

#include <cstdint>
#include <string>
#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief This object represents a bot command.
 *
 * https://core.telegram.org/bots/api#botcommand
 * @ingroup types
 */
class BotCommand {
public:
    using Ptr = std::shared_ptr<BotCommand>;
    BotCommand() = default;
    virtual ~BotCommand() = default;

    /**
     * @brief command label.
     */
    std::string command;

    /**
     * @brief description label.
     */
    std::string description;

    /**
     * @brief Optional. True, if the command sends an ephemeral message, which can be seen only by the sender of the message and the bot
     */
    std::optional<bool> isEphemeral;

};
}

#endif //TGBOT_BOTCOMMAND_H
