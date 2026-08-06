#ifndef TGBOT_INPUTMEDIAVOICENOTE_H
#define TGBOT_INPUTMEDIAVOICENOTE_H

#include "tgbot/types/InputMedia.h"
#include "tgbot/types/MessageEntity.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace TgBot {

/**
 * @brief Represents a voice message file to be sent.
 *
 * @ingroup types
 */
class InputMediaVoiceNote : public InputMedia {
public:
    using Ptr = std::shared_ptr<InputMediaVoiceNote>;

    constexpr static const char* TYPE = "voice_note";

    InputMediaVoiceNote() {
        type = TYPE;
    }

    /**
     * @brief File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass "attach://<file_attach_name>" to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files: https://core.telegram.org/bots/api#sending-files
     */
    std::string media;

    /**
     * @brief Optional. Caption of the voice message to be sent, 0-1024 characters after entities parsing
     */
    std::optional<std::string> caption;

    /**
     * @brief Optional. Mode for parsing entities in the voice message caption. See formatting options for more details.
     */
    std::optional<std::string> parseMode;

    /**
     * @brief Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
     */
    std::optional<std::vector<MessageEntity::Ptr>> captionEntities;

    /**
     * @brief Optional. Duration of the voice message in seconds
     */
    std::optional<std::int32_t> duration;

};
}

#endif //TGBOT_INPUTMEDIAVOICENOTE_H
