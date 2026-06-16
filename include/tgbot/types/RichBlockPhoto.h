#ifndef TGBOT_RICHBLOCKPHOTO_H
#define TGBOT_RICHBLOCKPHOTO_H

#include "tgbot/types/PhotoSize.h"
#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief A block with a photo, corresponding to the HTML tag <photo>.
 *
 * @ingroup types
 */
class RichBlockPhoto : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockPhoto>;

    constexpr static const char* TYPE = "photo";

    RichBlockPhoto() {
        type = TYPE;
    }

    /**
     * @brief Available sizes of the photo
     */
    std::vector<PhotoSize::Ptr> photo;

    /**
     * @brief Optional. True, if the media preview is covered by a spoiler animation
     */
    std::optional<bool> hasSpoiler;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKPHOTO_H
