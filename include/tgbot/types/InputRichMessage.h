#ifndef TGBOT_INPUTRICHMESSAGE_H
#define TGBOT_INPUTRICHMESSAGE_H

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief Describes a rich message to be sent. Exactly one of the fields html or markdown must be used.
 *
 * @ingroup types
 */
class InputRichMessage {
public:
    using Ptr = std::shared_ptr<InputRichMessage>;

    /**
     * @brief Optional. Content of the rich message to send described using HTML formatting. See rich message formatting options for more details.
     */
    std::optional<std::string> html;

    /**
     * @brief Optional. Content of the rich message to send described using Markdown formatting. See rich message formatting options for more details.
     */
    std::optional<std::string> markdown;

    /**
     * @brief Optional. Pass True if the rich message must be shown right-to-left
     */
    std::optional<bool> isRtl;

    /**
     * @brief Optional. Pass True to skip automatic detection of entities (e.g., URLs, email addresses, username mentions, hashtags, cashtags, bot commands, or phone numbers) in the text
     */
    std::optional<bool> skipEntityDetection;

};
}

#endif //TGBOT_INPUTRICHMESSAGE_H
