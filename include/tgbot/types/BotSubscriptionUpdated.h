#ifndef TGBOT_BOTSUBSCRIPTIONUPDATED_H
#define TGBOT_BOTSUBSCRIPTIONUPDATED_H

#include "tgbot/types/User.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object contains information about changes to a user payment subscription toward the current bot.
 *
 * @ingroup types
 */
class BotSubscriptionUpdated {
public:
    using Ptr = std::shared_ptr<BotSubscriptionUpdated>;

    /**
     * @brief User who subscribed for payments toward the bot
     */
    User::Ptr user;

    /**
     * @brief Bot-specified invoice payload
     */
    std::string invoicePayload;

    /**
     * @brief The new state of the subscription. Currently, it can be one of "canceled" if the user canceled the subscription, "active" if the user re-enabled a previously canceled subscription, or "failed" if payment for the subscription failed.
     */
    std::string state;

};
}

#endif //TGBOT_BOTSUBSCRIPTIONUPDATED_H
