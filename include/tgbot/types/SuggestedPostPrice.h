#ifndef TGBOT_SUGGESTEDPOSTPRICE_H
#define TGBOT_SUGGESTEDPOSTPRICE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes the price of a suggested post.
 *
 * @ingroup types
 */
class SuggestedPostPrice {
public:
    using Ptr = std::shared_ptr<SuggestedPostPrice>;

    /**
     * @brief Currency in which the post will be paid. Currently, must be one of "XTR" for Telegram Stars or "TON" for toncoins
     */
    std::string currency;

    /**
     * @brief The amount of the currency that will be paid for the post in the smallest units of the currency, i.e. Telegram Stars or nanotoncoins. Currently, price in Telegram Stars must be between 5 and 100000, and price in nanotoncoins must be between 10000000 and 10000000000000.
     */
    std::int64_t amount;

};
}

#endif //TGBOT_SUGGESTEDPOSTPRICE_H
