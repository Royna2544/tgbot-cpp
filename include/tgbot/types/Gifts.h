#ifndef TGBOT_GIFTS_H
#define TGBOT_GIFTS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Gift.h"

namespace TgBot {

/**
 * @brief This object represent a list of gifts.
 *
 * @ingroup types
 */
class Gifts {
public:
    using Ptr = std::shared_ptr<Gifts>;

    /**
     * @brief The list of gifts
     */
    std::vector<Gift::Ptr> gifts;

};
}

#endif //TGBOT_GIFTS_H
