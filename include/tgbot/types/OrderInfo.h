#ifndef TGBOT_ORDERINFO_H
#define TGBOT_ORDERINFO_H

#include "tgbot/types/ShippingAddress.h"

#include <optional>
#include <string>
#include <memory>

namespace TgBot {

/**
 * @brief This object represents information about an order.
 *
 * https://core.telegram.org/bots/api#orderinfo
 *
 * @ingroup types
 */
class OrderInfo {
public:
    using Ptr = std::shared_ptr<OrderInfo>;

    /**
     * @brief Optional. User name.
     */
    std::optional<std::string> name;

    /**
     * @brief Optional. User's phone number.
     */
    std::optional<std::string> phoneNumber;

    /**
     * @brief Optional. User email.
     */
    std::optional<std::string> email;

    /**
     * @brief Optional. User shipping address.
     */
    ShippingAddress::Ptr shippingAddress;
};

}

#endif //TGBOT_ORDERINFO_H
