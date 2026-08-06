#ifndef TGBOT_INPUTRICHBLOCKPHOTO_H
#define TGBOT_INPUTRICHBLOCKPHOTO_H

#include "tgbot/types/InputMediaPhoto.h"
#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a photo, corresponding to the HTML tag <img>.
 *
 * @ingroup types
 */
class InputRichBlockPhoto : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockPhoto>;

    constexpr static const char* TYPE = "photo";

    InputRichBlockPhoto() {
        type = TYPE;
    }

    /**
     * @brief The photo. Caption is ignored.
     */
    InputMediaPhoto::Ptr photo;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKPHOTO_H
