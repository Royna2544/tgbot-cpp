#ifndef TGBOT_SHIPPINGOPTION_H
#define TGBOT_SHIPPINGOPTION_H

#include "tgbot/types/LabeledPrice.h"

#include <string>
#include <memory>
#include <vector>

namespace TgBot {

/**
 * @brief This object represents one shipping option.
 *
 * https://core.telegram.org/bots/api#shippingoption
 *
 * @ingroup types
 */
class ShippingOption {
public:
    using Ptr = std::shared_ptr<ShippingOption>;

    /**
     * @brief Shipping option identifier.
     */
    std::string id;

    /**
     * @brief Option title.
     */
    std::string title;

    /**
     * @brief List of price options.
     */
    std::vector<LabeledPrice::Ptr> prices;
};
}

#endif //TGBOT_SHIPPINGOPTION_H
