#ifndef TGBOT_BOTNAME_H
#define TGBOT_BOTNAME_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object represents the bot's name.
 *
 * @ingroup types
 */
class BotName {
public:
    using Ptr = std::shared_ptr<BotName>;

    /**
     * @brief The bot's name
     */
    std::string name;
};
}

#endif //TGBOT_BOTNAME_H
