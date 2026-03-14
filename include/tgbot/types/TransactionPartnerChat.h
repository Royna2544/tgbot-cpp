#ifndef TGBOT_TRANSACTIONPARTNERCHAT_H
#define TGBOT_TRANSACTIONPARTNERCHAT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"

namespace TgBot {

/**
 * @brief Describes a transaction with a chat.
 *
 * @ingroup types
 */
class TransactionPartnerChat : public TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartnerChat>;
    constexpr static const char* TYPE = "chat";

    TransactionPartnerChat() {
        type = TYPE;
    }

    /**
     * @brief Information about the chat
     */
    Chat::Ptr chat;

    /**
     * @brief Optional. The gift sent to the chat by the bot
     */
    std::optional<Gift::Ptr> gift;

};
}

#endif //TGBOT_TRANSACTIONPARTNERCHAT_H
