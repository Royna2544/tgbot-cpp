#ifndef TGBOT_STARAMOUNT_H
#define TGBOT_STARAMOUNT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes an amount of Telegram Stars.
 *
 * @ingroup types
 */
class StarAmount {
public:
    using Ptr = std::shared_ptr<StarAmount>;

    /**
     * @brief Integer amount of Telegram Stars, rounded to 0; can be negative
     */
    std::int64_t amount;

    /**
     * @brief Optional. The number of 1/1000000000 shares of Telegram Stars; from -999999999 to 999999999; can be negative if and only if amount is non-positive
     */
    std::optional<std::int64_t> nanostarAmount;

};
}

#endif //TGBOT_STARAMOUNT_H
