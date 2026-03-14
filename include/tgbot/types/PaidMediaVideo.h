#ifndef TGBOT_PAIDMEDIAVIDEO_H
#define TGBOT_PAIDMEDIAVIDEO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/PaidMedia.h"
#include "tgbot/types/Video.h"

namespace TgBot {

/**
 * @brief The paid media is a video.
 *
 * @ingroup types
 */
class PaidMediaVideo : public PaidMedia {
public:
    using Ptr = std::shared_ptr<PaidMediaVideo>;
    constexpr static const char* TYPE = "video";

    PaidMediaVideo() {
        type = TYPE;
    }

    /**
     * @brief The video
     */
    Video::Ptr video;

};
}

#endif //TGBOT_PAIDMEDIAVIDEO_H
