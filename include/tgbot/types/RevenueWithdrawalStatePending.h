#ifndef TGBOT_REVENUEWITHDRAWALSTATEPENDING_H
#define TGBOT_REVENUEWITHDRAWALSTATEPENDING_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/RevenueWithdrawalState.h"

namespace TgBot {

/**
 * @brief The withdrawal is in progress.
 *
 * @ingroup types
 */
class RevenueWithdrawalStatePending : public RevenueWithdrawalState {
public:
    using Ptr = std::shared_ptr<RevenueWithdrawalStatePending>;
    constexpr static const char* TYPE = "pending";

    RevenueWithdrawalStatePending() {
        type = TYPE;
    }
};
}

#endif //TGBOT_REVENUEWITHDRAWALSTATEPENDING_H
