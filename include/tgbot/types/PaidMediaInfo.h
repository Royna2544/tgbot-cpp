#ifndef TGBOT_PAIDMEDIAINFO_H
#define TGBOT_PAIDMEDIAINFO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/PaidMedia.h"

namespace TgBot {

/**
 * @brief Describes the paid media added to a message.
 *
 * @ingroup types
 */
class PaidMediaInfo {
public:
    using Ptr = std::shared_ptr<PaidMediaInfo>;

    /**
     * @brief The number of Telegram Stars that must be paid to buy access to the media
     */
    std::int64_t starCount;

    /**
     * @brief Information about the paid media
     */
    std::vector<PaidMedia::Ptr> paidMedia;

};
}

#endif //TGBOT_PAIDMEDIAINFO_H
