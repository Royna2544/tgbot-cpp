#ifndef TGBOT_REVENUEWITHDRAWALSTATE_H
#define TGBOT_REVENUEWITHDRAWALSTATE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the state of a revenue withdrawal operation. Currently, it can be one of - RevenueWithdrawalStatePending - RevenueWithdrawalStateSucceeded - RevenueWithdrawalStateFailed
 *
 * @ingroup types
 */
class RevenueWithdrawalState {
public:
    using Ptr = std::shared_ptr<RevenueWithdrawalState>;

    std::string type;

};
}

#endif //TGBOT_REVENUEWITHDRAWALSTATE_H
