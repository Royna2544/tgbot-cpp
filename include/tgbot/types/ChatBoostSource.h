#ifndef TGBOT_CHATBOOSTSOURCE_H
#define TGBOT_CHATBOOSTSOURCE_H

#include "tgbot/types/User.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object describes the source of a chat boost.
 *
 * It can be one of
 * - ChatBoostSourcePremium
 * - ChatBoostSourceGiftCode
 * - ChatBoostSourceGiveaway
 *
 * @ingroup types
 */
class ChatBoostSource {

public:
    using Ptr = std::shared_ptr<ChatBoostSource>;

    ChatBoostSource() = default;
    virtual ~ChatBoostSource() = default;

    /**
     * @brief Source of the boost
     */
    std::string source;

    /**
     * @brief User
     */
    User::Ptr user;
};
}

#endif //TGBOT_CHATBOOSTSOURCE_H
