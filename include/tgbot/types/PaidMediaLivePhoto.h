#ifndef TGBOT_PAIDMEDIALIVEPHOTO_H
#define TGBOT_PAIDMEDIALIVEPHOTO_H

#include "tgbot/types/LivePhoto.h"
#include "tgbot/types/PaidMedia.h"

#include <memory>

namespace TgBot {

/**
 * @brief The paid media is a live photo.
 *
 * @ingroup types
 */
class PaidMediaLivePhoto : public PaidMedia {
public:
    using Ptr = std::shared_ptr<PaidMediaLivePhoto>;

    constexpr static const char* TYPE = "live_photo";

    PaidMediaLivePhoto() {
        type = TYPE;
    }

    /**
     * @brief The photo
     */
    LivePhoto::Ptr livePhoto;

};
}

#endif //TGBOT_PAIDMEDIALIVEPHOTO_H
