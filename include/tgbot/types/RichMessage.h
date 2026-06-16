#ifndef TGBOT_RICHMESSAGE_H
#define TGBOT_RICHMESSAGE_H

#include "tgbot/types/RichBlock.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief Rich formatted message.
 *
 * @ingroup types
 */
class RichMessage {
public:
    using Ptr = std::shared_ptr<RichMessage>;

    /**
     * @brief Content of the message
     */
    std::vector<RichBlock::Ptr> blocks;

    /**
     * @brief Optional. True, if the rich message must be shown right-to-left
     */
    std::optional<bool> isRtl;

};
}

#endif //TGBOT_RICHMESSAGE_H
