#ifndef TGBOT_PAIDMEDIAPURCHASED_H
#define TGBOT_PAIDMEDIAPURCHASED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/User.h"

namespace TgBot {

/**
 * @brief This object contains information about a paid media purchase.
 *
 * @ingroup types
 */
class PaidMediaPurchased {
public:
    using Ptr = std::shared_ptr<PaidMediaPurchased>;

    /**
     * @brief User who purchased the media
     */
    User::Ptr from;

    /**
     * @brief Bot-specified paid media payload
     */
    std::string paidMediaPayload;

};
}

#endif //TGBOT_PAIDMEDIAPURCHASED_H
