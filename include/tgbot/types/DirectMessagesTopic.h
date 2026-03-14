#ifndef TGBOT_DIRECTMESSAGESTOPIC_H
#define TGBOT_DIRECTMESSAGESTOPIC_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/User.h"

namespace TgBot {

/**
 * @brief Describes a topic of a direct messages chat.
 *
 * @ingroup types
 */
class DirectMessagesTopic {
public:
    using Ptr = std::shared_ptr<DirectMessagesTopic>;

    /**
     * @brief Unique identifier of the topic. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier.
     */
    std::int64_t topicId;

    /**
     * @brief Optional. Information about the user that created the topic. Currently, it is always present
     */
    std::optional<User::Ptr> user;

};
}

#endif //TGBOT_DIRECTMESSAGESTOPIC_H
