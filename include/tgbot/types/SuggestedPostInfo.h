#ifndef TGBOT_SUGGESTEDPOSTINFO_H
#define TGBOT_SUGGESTEDPOSTINFO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/SuggestedPostPrice.h"

namespace TgBot {

/**
 * @brief Contains information about a suggested post.
 *
 * @ingroup types
 */
class SuggestedPostInfo {
public:
    using Ptr = std::shared_ptr<SuggestedPostInfo>;

    /**
     * @brief State of the suggested post. Currently, it can be one of "pending", "approved", "declined".
     */
    std::string state;

    /**
     * @brief Optional. Proposed price of the post. If the field is omitted, then the post is unpaid.
     */
    std::optional<SuggestedPostPrice::Ptr> price;

    /**
     * @brief Optional. Proposed send date of the post. If the field is omitted, then the post can be published at any time within 30 days at the sole discretion of the user or administrator who approves it.
     */
    std::optional<std::int64_t> sendDate;

};
}

#endif //TGBOT_SUGGESTEDPOSTINFO_H
