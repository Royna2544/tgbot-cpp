#ifndef TGBOT_INPUTMEDIASTICKER_H
#define TGBOT_INPUTMEDIASTICKER_H

#include "tgbot/types/InputMedia.h"

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief Represents a sticker file to be sent.
 *
 * @ingroup types
 */
class InputMediaSticker : public InputMedia {
public:
    using Ptr = std::shared_ptr<InputMediaSticker>;

    constexpr static const char* TYPE = "sticker";

    InputMediaSticker() {
        type = TYPE;
    }

    /**
     * @brief File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a .WEBP sticker from the Internet, or pass "attach://<file_attach_name>" to upload a new .WEBP, .TGS, or .WEBM sticker using multipart/form-data under <file_attach_name> name. More information on Sending Files: https://core.telegram.org/bots/api#sending-files
     */
    std::string media;

    /**
     * @brief Optional. Emoji associated with the sticker; only for just uploaded stickers
     */
    std::optional<std::string> emoji;

};
}

#endif //TGBOT_INPUTMEDIASTICKER_H
