#ifndef TGBOT_INPUTSTORYCONTENTVIDEO_H
#define TGBOT_INPUTSTORYCONTENTVIDEO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/InputStoryContent.h"

namespace TgBot {

/**
 * @brief Describes a video to post as a story.
 *
 * @ingroup types
 */
class InputStoryContentVideo : public InputStoryContent {
public:
    using Ptr = std::shared_ptr<InputStoryContentVideo>;
    constexpr static const char* TYPE = "video";

    InputStoryContentVideo() {
        type = TYPE;
    }

    /**
     * @brief The video to post as a story. The video must be of the size 720x1280, streamable, encoded with H.265 codec, with key frames added each second in the MPEG4 format, and must not exceed 30 MB. The video can't be reused and can only be uploaded as a new file, so you can pass "attach://<file_attach_name>" if the video was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files: https://core.telegram.org/bots/api#sending-files
     */
    std::string video;

    /**
     * @brief Optional. Precise duration of the video in seconds; 0-60
     */
    std::optional<float> duration;

    /**
     * @brief Optional. Timestamp in seconds of the frame that will be used as the static cover for the story. Defaults to 0.0.
     */
    std::optional<float> coverFrameTimestamp;

    /**
     * @brief Optional. Pass True if the video has no sound
     */
    std::optional<bool> isAnimation;

};
}

#endif //TGBOT_INPUTSTORYCONTENTVIDEO_H
