#ifndef TGBOT_PAIDMEDIAPHOTO_H
#define TGBOT_PAIDMEDIAPHOTO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/PaidMedia.h"
#include "tgbot/types/PhotoSize.h"

namespace TgBot {

/**
 * @brief The paid media is a photo.
 *
 * @ingroup types
 */
class PaidMediaPhoto : public PaidMedia {
public:
    using Ptr = std::shared_ptr<PaidMediaPhoto>;
    constexpr static const char* TYPE = "photo";

    PaidMediaPhoto() {
        type = TYPE;
    }

    /**
     * @brief The photo
     */
    std::vector<PhotoSize::Ptr> photo;

};
}

#endif //TGBOT_PAIDMEDIAPHOTO_H
