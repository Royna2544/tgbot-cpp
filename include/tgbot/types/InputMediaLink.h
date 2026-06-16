#ifndef TGBOT_INPUTMEDIALINK_H
#define TGBOT_INPUTMEDIALINK_H

#include "tgbot/types/InputMedia.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Represents an HTTP link to be sent.
 *
 * @ingroup types
 */
class InputMediaLink : public InputMedia {
public:
    using Ptr = std::shared_ptr<InputMediaLink>;

    constexpr static const char* TYPE = "link";

    InputMediaLink() {
        type = TYPE;
    }

    /**
     * @brief HTTP URL of the link
     */
    std::string url;

};
}

#endif //TGBOT_INPUTMEDIALINK_H
