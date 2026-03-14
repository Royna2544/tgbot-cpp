#ifndef TGBOT_PAIDMESSAGEPRICECHANGED_H
#define TGBOT_PAIDMESSAGEPRICECHANGED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes a service message about a change in the price of paid messages within a chat.
 *
 * @ingroup types
 */
class PaidMessagePriceChanged {
public:
    using Ptr = std::shared_ptr<PaidMessagePriceChanged>;

    /**
     * @brief The new number of Telegram Stars that must be paid by non-administrator users of the supergroup chat for each sent message
     */
    std::int64_t paidMessageStarCount;

};
}

#endif //TGBOT_PAIDMESSAGEPRICECHANGED_H
