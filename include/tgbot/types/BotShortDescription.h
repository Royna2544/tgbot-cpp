#ifndef TGBOT_BOTSHORTDESCRIPTION_H
#define TGBOT_BOTSHORTDESCRIPTION_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object represents the bot's short description.
 *
 * @ingroup types
 */
class BotShortDescription {
public:
    using Ptr = std::shared_ptr<BotShortDescription>;

    /**
     * @brief The bot's short description
     */
    std::string shortDescription;
};
}

#endif //TGBOT_BOTSHORTDESCRIPTION_H
