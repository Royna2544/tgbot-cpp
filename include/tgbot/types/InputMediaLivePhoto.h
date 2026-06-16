#ifndef TGBOT_INPUTMEDIALIVEPHOTO_H
#define TGBOT_INPUTMEDIALIVEPHOTO_H

#include "tgbot/types/InputMedia.h"
#include "tgbot/types/MessageEntity.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace TgBot {

/**
 * @brief Represents a live photo to be sent.
 *
 * @ingroup types
 */
class InputMediaLivePhoto : public InputMedia {
public:
    using Ptr = std::shared_ptr<InputMediaLivePhoto>;

    constexpr static const char* TYPE = "live_photo";

    InputMediaLivePhoto() {
        type = TYPE;
    }

    /**
     * @brief Video of the live photo to send. Pass a file_id to send a file that exists on the Telegram servers (recommended) or pass "attach://<file_attach_name>" to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files: https://core.telegram.org/bots/api#sending-files. Sending live photos by a URL is currently unsupported.
     */
    std::string media;

    /**
     * @brief The static photo to send. Pass a file_id to send a file that exists on the Telegram servers (recommended) or pass "attach://<file_attach_name>" to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files: https://core.telegram.org/bots/api#sending-files. Sending live photos by a URL is currently unsupported.
     */
    std::string photo;

    /**
     * @brief Optional. Caption of the live photo to be sent, 0-1024 characters after entities parsing
     */
    std::optional<std::string> caption;

    /**
     * @brief Optional. Mode for parsing entities in the live photo caption. See formatting options for more details.
     */
    std::optional<std::string> parseMode;

    /**
     * @brief Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
     */
    std::optional<std::vector<MessageEntity::Ptr>> captionEntities;

    /**
     * @brief Optional. Pass True, if the caption must be shown above the message media
     */
    std::optional<bool> showCaptionAboveMedia;

    /**
     * @brief Optional. Pass True if the live photo needs to be covered with a spoiler animation
     */
    std::optional<bool> hasSpoiler;

};
}

#endif //TGBOT_INPUTMEDIALIVEPHOTO_H
