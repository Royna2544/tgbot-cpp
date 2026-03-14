#ifndef TGBOT_USERRATING_H
#define TGBOT_USERRATING_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the rating of a user based on their Telegram Star spendings.
 *
 * @ingroup types
 */
class UserRating {
public:
    using Ptr = std::shared_ptr<UserRating>;

    /**
     * @brief Current level of the user, indicating their reliability when purchasing digital goods and services. A higher level suggests a more trustworthy customer; a negative level is likely reason for concern.
     */
    std::int64_t level;

    /**
     * @brief Numerical value of the user's rating; the higher the rating, the better
     */
    std::int64_t rating;

    /**
     * @brief The rating value required to get the current level
     */
    std::int64_t currentLevelRating;

    /**
     * @brief Optional. The rating value required to get to the next level; omitted if the maximum level was reached
     */
    std::optional<std::int64_t> nextLevelRating;

};
}

#endif //TGBOT_USERRATING_H
