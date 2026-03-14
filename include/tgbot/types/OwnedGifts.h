#ifndef TGBOT_OWNEDGIFTS_H
#define TGBOT_OWNEDGIFTS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/OwnedGift.h"

namespace TgBot {

/**
 * @brief Contains the list of gifts received and owned by a user or a chat.
 *
 * @ingroup types
 */
class OwnedGifts {
public:
    using Ptr = std::shared_ptr<OwnedGifts>;

    /**
     * @brief The total number of gifts owned by the user or the chat
     */
    std::int64_t totalCount;

    /**
     * @brief The list of gifts
     */
    std::vector<OwnedGift::Ptr> gifts;

    /**
     * @brief Optional. Offset for the next request. If empty, then there are no more results
     */
    std::optional<std::string> nextOffset;

};
}

#endif //TGBOT_OWNEDGIFTS_H
