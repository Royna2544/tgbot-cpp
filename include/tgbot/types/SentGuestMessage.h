#ifndef TGBOT_SENTGUESTMESSAGE_H
#define TGBOT_SENTGUESTMESSAGE_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Describes an inline message sent by a guest bot.
 *
 * @ingroup types
 */
class SentGuestMessage {
public:
    using Ptr = std::shared_ptr<SentGuestMessage>;

    /**
     * @brief Identifier of the sent inline message
     */
    std::string inlineMessageId;

};
}

#endif //TGBOT_SENTGUESTMESSAGE_H
