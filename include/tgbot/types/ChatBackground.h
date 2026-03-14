#ifndef TGBOT_CHATBACKGROUND_H
#define TGBOT_CHATBACKGROUND_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundType.h"

namespace TgBot {

/**
 * @brief This object represents a chat background.
 *
 * @ingroup types
 */
class ChatBackground {
public:
    using Ptr = std::shared_ptr<ChatBackground>;

    /**
     * @brief Type of the background
     */
    BackgroundType::Ptr type;

};
}

#endif //TGBOT_CHATBACKGROUND_H
