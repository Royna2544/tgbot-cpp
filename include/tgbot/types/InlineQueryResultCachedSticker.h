#ifndef TGBOT_INLINEQUERYRESULTCACHEDSTICKER_H
#define TGBOT_INLINEQUERYRESULTCACHEDSTICKER_H

#include "tgbot/types/InlineQueryResult.h"
#include "tgbot/types/InputMessageContent.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Represents a link to a sticker stored on the Telegram servers.
 *
 * By default, this sticker will be sent by the user.
 * Alternatively, you can use inputMessageContent to send a message with the specified content instead of the sticker.
 *
 * @ingroup types
 */
class InlineQueryResultCachedSticker : public InlineQueryResult {

public:
    static constexpr std::string_view TYPE = "sticker";

    using Ptr = std::shared_ptr<InlineQueryResultCachedSticker>;

    InlineQueryResultCachedSticker() {
        this->type = TYPE;
	isCached = true;
    }

    /**
     * @brief A valid file identifier of the sticker
     */
    std::string stickerFileId;

    /**
     * @brief Optional. Content of the message to be sent instead of the sticker
     */
    InputMessageContent::Ptr inputMessageContent;
};
}

#endif //TGBOT_INLINEQUERYRESULTCACHEDSTICKER_H
