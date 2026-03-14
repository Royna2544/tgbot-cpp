#ifndef TGBOT_BACKGROUNDTYPECHATTHEME_H
#define TGBOT_BACKGROUNDTYPECHATTHEME_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/BackgroundType.h"

namespace TgBot {

/**
 * @brief The background is taken directly from a built-in chat theme.
 *
 * @ingroup types
 */
class BackgroundTypeChatTheme : public BackgroundType {
public:
    using Ptr = std::shared_ptr<BackgroundTypeChatTheme>;
    constexpr static const char* TYPE = "chat_theme";

    BackgroundTypeChatTheme() {
        type = TYPE;
    }

    /**
     * @brief Name of the chat theme, which is usually an emoji
     */
    std::string themeName;

};
}

#endif //TGBOT_BACKGROUNDTYPECHATTHEME_H
