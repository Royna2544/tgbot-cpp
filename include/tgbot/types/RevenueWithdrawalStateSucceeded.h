#ifndef TGBOT_REVENUEWITHDRAWALSTATESUCCEEDED_H
#define TGBOT_REVENUEWITHDRAWALSTATESUCCEEDED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/RevenueWithdrawalState.h"

namespace TgBot {

/**
 * @brief The withdrawal succeeded.
 *
 * @ingroup types
 */
class RevenueWithdrawalStateSucceeded : public RevenueWithdrawalState {
public:
    using Ptr = std::shared_ptr<RevenueWithdrawalStateSucceeded>;
    constexpr static const char* TYPE = "succeeded";

    RevenueWithdrawalStateSucceeded() {
        type = TYPE;
    }

    /**
     * @brief Date the withdrawal was completed in Unix time
     */
    std::int64_t date;

    /**
     * @brief An HTTPS URL that can be used to see transaction details
     */
    std::string url;

};
}

#endif //TGBOT_REVENUEWITHDRAWALSTATESUCCEEDED_H
