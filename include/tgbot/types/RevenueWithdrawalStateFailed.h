#ifndef TGBOT_REVENUEWITHDRAWALSTATEFAILED_H
#define TGBOT_REVENUEWITHDRAWALSTATEFAILED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/RevenueWithdrawalState.h"

namespace TgBot {

/**
 * @brief The withdrawal failed and the transaction was refunded.
 *
 * @ingroup types
 */
class RevenueWithdrawalStateFailed : public RevenueWithdrawalState {
public:
    using Ptr = std::shared_ptr<RevenueWithdrawalStateFailed>;
    constexpr static const char* TYPE = "failed";

    RevenueWithdrawalStateFailed() {
        type = TYPE;
    }
};
}

#endif //TGBOT_REVENUEWITHDRAWALSTATEFAILED_H
