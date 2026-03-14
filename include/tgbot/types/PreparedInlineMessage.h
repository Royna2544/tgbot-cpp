#ifndef TGBOT_PREPAREDINLINEMESSAGE_H
#define TGBOT_PREPAREDINLINEMESSAGE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes an inline message to be sent by a user of a Mini App.
 *
 * @ingroup types
 */
class PreparedInlineMessage {
public:
    using Ptr = std::shared_ptr<PreparedInlineMessage>;

    /**
     * @brief Unique identifier of the prepared message
     */
    std::string id;

    /**
     * @brief Expiration date of the prepared message, in Unix time. Expired prepared messages can no longer be used
     */
    std::int64_t expirationDate;

};
}

#endif //TGBOT_PREPAREDINLINEMESSAGE_H
