#ifndef TGBOT_COMMUNITY_H
#define TGBOT_COMMUNITY_H

#include <cstdint>
#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Represents a community (a group of chats).
 *
 * @ingroup types
 */
class Community {
public:
    using Ptr = std::shared_ptr<Community>;

    /**
     * @brief Unique identifier for this community. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
     */
    std::int64_t id;

    /**
     * @brief Name of the community
     */
    std::string name;

};
}

#endif //TGBOT_COMMUNITY_H
