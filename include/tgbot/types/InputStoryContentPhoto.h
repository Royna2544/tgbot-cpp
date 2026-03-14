#ifndef TGBOT_INPUTSTORYCONTENTPHOTO_H
#define TGBOT_INPUTSTORYCONTENTPHOTO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/InputStoryContent.h"

namespace TgBot {

/**
 * @brief Describes a photo to post as a story.
 *
 * @ingroup types
 */
class InputStoryContentPhoto : public InputStoryContent {
public:
    using Ptr = std::shared_ptr<InputStoryContentPhoto>;
    constexpr static const char* TYPE = "photo";

    InputStoryContentPhoto() {
        type = TYPE;
    }

    /**
     * @brief The photo to post as a story. The photo must be of the size 1080x1920 and must not exceed 10 MB. The photo can't be reused and can only be uploaded as a new file, so you can pass "attach://<file_attach_name>" if the photo was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files: https://core.telegram.org/bots/api#sending-files
     */
    std::string photo;

};
}

#endif //TGBOT_INPUTSTORYCONTENTPHOTO_H
