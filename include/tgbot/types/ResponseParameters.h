#ifndef TGBOT_RESPONSEPARAMETERS_H
#define TGBOT_RESPONSEPARAMETERS_H

#include <cstdint>
#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief Contains information about why a request was unsuccessfull.
 *
 * @ingroup types
 */
class ResponseParameters {
public:
    using Ptr = std::shared_ptr<ResponseParameters>;

    /**
     * @brief Optional. The group has been migrated to a supergroup with the specified identifier.
     *
     * This number may be greater than 32 bits and some programming languages may have
     * difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a
     * signed 64 bit integer or double-precision float type are safe for storing this identifier.
     */
    std::optional<std::int64_t> migrateToChatId;

    /**
     * @brief Optional. In case of exceeding flood control, the number of seconds left to wait before the request can be repeated
     */
    std::optional<std::int32_t> retryAfter;
};
}

#endif //TGBOT_RESPONSEPARAMETERS_H
