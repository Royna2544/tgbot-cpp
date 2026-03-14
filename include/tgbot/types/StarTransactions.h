#ifndef TGBOT_STARTRANSACTIONS_H
#define TGBOT_STARTRANSACTIONS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/StarTransaction.h"

namespace TgBot {

/**
 * @brief Contains a list of Telegram Star transactions.
 *
 * @ingroup types
 */
class StarTransactions {
public:
    using Ptr = std::shared_ptr<StarTransactions>;

    /**
     * @brief The list of transactions
     */
    std::vector<StarTransaction::Ptr> transactions;

};
}

#endif //TGBOT_STARTRANSACTIONS_H
