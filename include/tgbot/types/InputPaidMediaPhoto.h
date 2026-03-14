#ifndef TGBOT_INPUTPAIDMEDIAPHOTO_H
#define TGBOT_INPUTPAIDMEDIAPHOTO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/InputPaidMedia.h"

namespace TgBot {

/**
 * @brief The paid media to send is a photo.
 *
 * @ingroup types
 */
class InputPaidMediaPhoto : public InputPaidMedia {
public:
    using Ptr = std::shared_ptr<InputPaidMediaPhoto>;
    constexpr static const char* TYPE = "photo";

    InputPaidMediaPhoto() {
        type = TYPE;
    }

    /**
     * @brief File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass "attach://<file_attach_name>" to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files: https://core.telegram.org/bots/api#sending-files
     */
    std::string media;

};
}

#endif //TGBOT_INPUTPAIDMEDIAPHOTO_H
