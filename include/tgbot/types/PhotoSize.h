#ifndef TGBOT_CPP_PHOTOSIZE_H
#define TGBOT_CPP_PHOTOSIZE_H

#include <cstdint>
#include <optional>
#include <string>
#include <memory>

namespace TgBot {

/**
 * @brief This object represents one size of a photo or a file / sticker thumbnail.
 *
 * @ingroup types
 */
class PhotoSize {

public:
    using Ptr = std::shared_ptr<PhotoSize>;

    /**
     * @brief Identifier for this file, which can be used to download or reuse the file
     */
    std::string fileId;

    /**
     * @brief Unique identifier for this file, which is supposed to be the same over time and for different bots.
     * Can't be used to download or reuse the file.
     */
    std::string fileUniqueId;

    /**
     * @brief Photo width
     */
    std::int32_t width;

    /**
     * @brief Photo height
     */
    std::int32_t height;

    /**
     * @brief Optional. File size
     */
    std::optional<std::int32_t> fileSize;
};
}

#endif //TGBOT_CPP_PHOTOSIZE_H
