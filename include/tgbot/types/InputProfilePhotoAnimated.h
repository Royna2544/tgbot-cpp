#ifndef TGBOT_INPUTPROFILEPHOTOANIMATED_H
#define TGBOT_INPUTPROFILEPHOTOANIMATED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/InputProfilePhoto.h"

namespace TgBot {

/**
 * @brief An animated profile photo in the MPEG4 format.
 *
 * @ingroup types
 */
class InputProfilePhotoAnimated : public InputProfilePhoto {
public:
    using Ptr = std::shared_ptr<InputProfilePhotoAnimated>;
    static constexpr const char* TYPE = "animated";

    InputProfilePhotoAnimated() {
        type = TYPE;
    }

    /**
     * @brief The animated profile photo. Profile photos can't be reused and can only be uploaded as a new file, so you can pass "attach://<file_attach_name>" if the photo was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files: https://core.telegram.org/bots/api#sending-files
     */
    std::string animation;

    /**
     * @brief Optional. Timestamp in seconds of the frame that will be used as the static profile photo. Defaults to 0.0.
     */
    std::optional<float> mainFrameTimestamp;

};
}

#endif //TGBOT_INPUTPROFILEPHOTOANIMATED_H
