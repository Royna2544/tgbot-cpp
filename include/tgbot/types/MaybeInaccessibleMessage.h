#ifndef TGBOT_MAYBEINACCESSIBLEMESSAGE_H
#define TGBOT_MAYBEINACCESSIBLEMESSAGE_H

#include "tgbot/types/Message.h"
#include "tgbot/types/InaccessibleMessage.h"

#include <memory>
#include <variant>

namespace TgBot {

/**
 * @brief This object describes a message that can be one of Message or InaccessibleMessage.
 *
 * @ingroup types
 */
using MaybeInaccessibleMessage = std::variant<Message::Ptr, InaccessibleMessage::Ptr>;

}

#endif //TGBOT_MAYBEINACCESSIBLEMESSAGE_H
