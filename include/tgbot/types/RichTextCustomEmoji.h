#ifndef TGBOT_RICHTEXTCUSTOMEMOJI_H
#define TGBOT_RICHTEXTCUSTOMEMOJI_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A custom emoji.
 *
 * @ingroup types
 */
class RichTextCustomEmoji : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextCustomEmoji>;

    constexpr static const char* TYPE = "custom_emoji";

    RichTextCustomEmoji() {
        type = TYPE;
    }

    /**
     * @brief Unique identifier of the custom emoji. Use getCustomEmojiStickers to get full information about the sticker.
     */
    std::string customEmojiId;

    /**
     * @brief Alternative emoji for the custom emoji
     */
    std::string alternativeText;

};
}

#endif //TGBOT_RICHTEXTCUSTOMEMOJI_H
