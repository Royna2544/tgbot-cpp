#ifndef TGBOT_BOTCOMMAND_H
#define TGBOT_BOTCOMMAND_H

#include <cstdint>
#include <string>
#include <memory>

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

};
}

#endif //TGBOT_BOTCOMMAND_H
