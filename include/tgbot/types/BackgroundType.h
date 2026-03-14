#ifndef TGBOT_BACKGROUNDTYPE_H
#define TGBOT_BACKGROUNDTYPE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the type of a background. Currently, it can be one of - BackgroundTypeFill - BackgroundTypeWallpaper - BackgroundTypePattern - BackgroundTypeChatTheme
 *
 * @ingroup types
 */
class BackgroundType {
public:
    using Ptr = std::shared_ptr<BackgroundType>;

    std::string type;

};
}

#endif //TGBOT_BACKGROUNDTYPE_H
