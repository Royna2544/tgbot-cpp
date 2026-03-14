#ifndef TGBOT_VIDEOQUALITY_H
#define TGBOT_VIDEOQUALITY_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object represents a video file of a specific quality.
 *
 * @ingroup types
 */
class VideoQuality {
public:
    using Ptr = std::shared_ptr<VideoQuality>;

    /**
     * @brief Identifier for this file, which can be used to download or reuse the file
     */
    std::string fileId;

    /**
     * @brief Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
     */
    std::string fileUniqueId;

    /**
     * @brief Video width
     */
    std::int64_t width;

    /**
     * @brief Video height
     */
    std::int64_t height;

    /**
     * @brief Codec that was used to encode the video, for example, "h264", "h265", or "av01"
     */
    std::string codec;

    /**
     * @brief Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.
     */
    std::optional<std::int64_t> fileSize;

};
}

#endif //TGBOT_VIDEOQUALITY_H
