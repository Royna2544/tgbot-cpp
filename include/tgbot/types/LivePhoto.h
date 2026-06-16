#ifndef TGBOT_LIVEPHOTO_H
#define TGBOT_LIVEPHOTO_H

#include "tgbot/types/PhotoSize.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace TgBot {

/**
 * @brief This object represents a live photo.
 *
 * @ingroup types
 */
class LivePhoto {
public:
    using Ptr = std::shared_ptr<LivePhoto>;

    /**
     * @brief Optional. Available sizes of the corresponding static photo
     */
    std::optional<std::vector<PhotoSize::Ptr>> photo;

    /**
     * @brief Identifier for the video file which can be used to download or reuse the file
     */
    std::string fileId;

    /**
     * @brief Unique identifier for the video file which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
     */
    std::string fileUniqueId;

    /**
     * @brief Video width as defined by the sender
     */
    std::int32_t width;

    /**
     * @brief Video height as defined by the sender
     */
    std::int32_t height;

    /**
     * @brief Duration of the video in seconds as defined by the sender
     */
    std::int32_t duration;

    /**
     * @brief Optional. MIME type of the file as defined by the sender
     */
    std::optional<std::string> mimeType;

    /**
     * @brief Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.
     */
    std::optional<std::int64_t> fileSize;

};
}

#endif //TGBOT_LIVEPHOTO_H
