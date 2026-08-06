#ifndef TGBOT_INPUTRICHMESSAGEMEDIA_H
#define TGBOT_INPUTRICHMESSAGEMEDIA_H

#include "tgbot/types/InputMedia.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Describes a media element embedded in an outgoing rich message.
 *
 * @ingroup types
 */
class InputRichMessageMedia {
public:
    using Ptr = std::shared_ptr<InputRichMessageMedia>;

    /**
     * @brief Unique identifier of the media used in a tg://photo?id=, tg://video?id=, or tg://audio?id= link. 1-64 characters, only A-Z, a-z, 0-9, _ and - are allowed.
     */
    std::string id;

    /**
     * @brief The media to be sent. Everything except the media itself and its properties is ignored.
     */
    InputMedia::Ptr media;

};
}

#endif //TGBOT_INPUTRICHMESSAGEMEDIA_H
