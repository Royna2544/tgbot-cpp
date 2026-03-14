#ifndef TGBOT_GIFTBACKGROUND_H
#define TGBOT_GIFTBACKGROUND_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the background of a gift.
 *
 * @ingroup types
 */
class GiftBackground {
public:
    using Ptr = std::shared_ptr<GiftBackground>;

    /**
     * @brief Center color of the background in RGB format
     */
    std::int64_t centerColor;

    /**
     * @brief Edge color of the background in RGB format
     */
    std::int64_t edgeColor;

    /**
     * @brief Text color of the background in RGB format
     */
    std::int64_t textColor;

};
}

#endif //TGBOT_GIFTBACKGROUND_H
