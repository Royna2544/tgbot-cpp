#ifndef TGBOT_SHIPPINGQUERY_H
#define TGBOT_SHIPPINGQUERY_H

#include "tgbot/types/User.h"
#include "tgbot/types/ShippingAddress.h"

#include <string>
#include <memory>

namespace TgBot {

/**
 * @brief This object contains information about an incoming shipping query.
 *
 * https://core.telegram.org/bots/api#shippingquery
 *
 * @ingroup types
 */
class ShippingQuery {
public:
    using Ptr = std::shared_ptr<ShippingQuery>;

    /**
     * @brief Unique query identifier.
     */
    std::string id;

    /**
     * @brief User who sent the query.
     */
    User::Ptr from;

    /**
     * @brief Bot specified invoice payload.
     */
    std::string invoicePayload;

    /**
     * @brief User specified shipping address.
     */
    ShippingAddress::Ptr shippingAddress;
};
}

#endif //TGBOT_SHIPPINGQUERY_H
