#ifndef TGBOT_COPYTEXTBUTTON_H
#define TGBOT_COPYTEXTBUTTON_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object represents an inline keyboard button that copies specified text to the clipboard.
 *
 * @ingroup types
 */
class CopyTextButton {
public:
    using Ptr = std::shared_ptr<CopyTextButton>;

    /**
     * @brief The text to be copied to the clipboard; 1-256 characters
     */
    std::string text;

};
}

#endif //TGBOT_COPYTEXTBUTTON_H
