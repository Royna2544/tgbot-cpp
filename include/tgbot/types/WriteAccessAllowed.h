#ifndef TGBOT_WRITEACCESSALLOWED_H
#define TGBOT_WRITEACCESSALLOWED_H

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief This object represents a service message about a user allowing a bot to write messages after adding it to the attachment menu, launching a Web App from a link, or accepting an explicit request from a Web App sent by the method requestWriteAccess.
 *
 * @ingroup types
 */
class WriteAccessAllowed {
public:
    using Ptr = std::shared_ptr<WriteAccessAllowed>;

    /**
     * @brief Optional. True, if the access was granted after the user accepted an explicit request from a Web App sent by the method requestWriteAccess (https://core.telegram.org/bots/webapps#initializing-mini-apps)
     */
    std::optional<bool> fromRequest;

    /**
     * @brief Optional. Name of the Web App, if the access was granted when the Web App was launched from a link
     */
    std::optional<std::string> webAppName;

    /**
     * @brief Optional. True, if the access was granted when the bot was added to the attachment or side menu
     */
    std::optional<bool> fromAttachmentMenu;
};
}

#endif //TGBOT_WRITEACCESSALLOWED_H
