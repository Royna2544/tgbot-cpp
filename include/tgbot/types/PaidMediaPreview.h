#ifndef TGBOT_PAIDMEDIAPREVIEW_H
#define TGBOT_PAIDMEDIAPREVIEW_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/PaidMedia.h"

namespace TgBot {

/**
 * @brief The paid media isn't available before the payment.
 *
 * @ingroup types
 */
class PaidMediaPreview : public PaidMedia {
public:
    using Ptr = std::shared_ptr<PaidMediaPreview>;
    constexpr static const char* TYPE = "preview";

    PaidMediaPreview() {
        type = TYPE;
    }

    /**
     * @brief Optional. Media width as defined by the sender
     */
    std::optional<std::int64_t> width;

    /**
     * @brief Optional. Media height as defined by the sender
     */
    std::optional<std::int64_t> height;

    /**
     * @brief Optional. Duration of the media in seconds as defined by the sender
     */
    std::optional<std::int64_t> duration;

};
}

#endif //TGBOT_PAIDMEDIAPREVIEW_H
