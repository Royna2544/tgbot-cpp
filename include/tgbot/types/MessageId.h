#ifndef TGBOT_CPP_MESSAGEID_H
#define TGBOT_CPP_MESSAGEID_H

#include <memory>
#include <cstdint>

namespace TgBot {

/**
 * @brief This object represents a unique message identifier.
 *
 * @ingroup types
 */
class MessageId {

public:
    using Ptr = std::shared_ptr<MessageId>;

    /**
     * @brief Unique message identifier
     */
    std::int32_t messageId;
};
}

#endif //TGBOT_CPP_MESSAGEID_H
