#ifndef TGBOT_SUGGESTEDPOSTPARAMETERS_H
#define TGBOT_SUGGESTEDPOSTPARAMETERS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/SuggestedPostPrice.h"

namespace TgBot {

/**
 * @brief Contains parameters of a post that is being suggested by the bot.
 *
 * @ingroup types
 */
class SuggestedPostParameters {
public:
    using Ptr = std::shared_ptr<SuggestedPostParameters>;

    /**
     * @brief Optional. Proposed price for the post. If the field is omitted, then the post is unpaid.
     */
    std::optional<SuggestedPostPrice::Ptr> price;

    /**
     * @brief Optional. Proposed send date of the post. If specified, then the date must be between 300 second and 2678400 seconds (30 days) in the future. If the field is omitted, then the post can be published at any time within 30 days at the sole discretion of the user who approves it.
     */
    std::optional<std::int64_t> sendDate;

};
}

#endif //TGBOT_SUGGESTEDPOSTPARAMETERS_H
