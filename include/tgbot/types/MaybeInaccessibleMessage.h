#ifndef TGBOT_MAYBEINACCESSIBLEMESSAGE_H
#define TGBOT_MAYBEINACCESSIBLEMESSAGE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

class Message;
class InaccessibleMessage;

/**
 * @brief This object describes a message that can be inaccessible to the bot. It can be one of - Message - InaccessibleMessage
 *
 * @ingroup types
 */
using MaybeInaccessibleMessage = std::variant<std::shared_ptr<Message>, std::shared_ptr<InaccessibleMessage>>;

#endif //TGBOT_MAYBEINACCESSIBLEMESSAGE_H
