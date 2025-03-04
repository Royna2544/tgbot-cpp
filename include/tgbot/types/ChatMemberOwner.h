#ifndef TGBOT_CHATMEMBEROWNER_H
#define TGBOT_CHATMEMBEROWNER_H

#include "tgbot/types/ChatMember.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Represents a chat member that owns the chat and has all administrator privileges.
 *
 * @ingroup types
 */
class ChatMemberOwner : public ChatMember {
public:
    static constexpr std::string_view STATUS = "creator";

    using Ptr = std::shared_ptr<ChatMemberOwner>;

    ChatMemberOwner() {
        this->status = STATUS;
    }

    /**
     * @brief Custom title for this user
     */
    std::string customTitle;

    /**
     * @brief True, if the user's presence in the chat is hidden
     */
    bool isAnonymous{};
};
}

#endif //TGBOT_CHATMEMBEROWNER_H
