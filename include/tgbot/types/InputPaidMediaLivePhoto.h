#ifndef TGBOT_INPUTPAIDMEDIALIVEPHOTO_H
#define TGBOT_INPUTPAIDMEDIALIVEPHOTO_H

#include "tgbot/types/InputPaidMedia.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief The paid media to send is a live photo.
 *
 * @ingroup types
 */
class InputPaidMediaLivePhoto : public InputPaidMedia {
public:
    using Ptr = std::shared_ptr<InputPaidMediaLivePhoto>;

    constexpr static const char* TYPE = "live_photo";

    InputPaidMediaLivePhoto() {
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

};
}

#endif //TGBOT_INPUTPAIDMEDIALIVEPHOTO_H
