#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatMemberUpdated) {
    auto result = std::make_shared<ChatMemberUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    result->from = parseRequired<User>(data, "from");
    parse(data, "date", &result->date);
    result->oldChatMember = parseRequired<ChatMember>(data, "old_chat_member");
    result->newChatMember = parseRequired<ChatMember>(data, "new_chat_member");
    result->inviteLink = parse<ChatInviteLink>(data, "invite_link");
    parse(data, "via_join_request", &result->viaJoinRequest);
    parse(data, "via_chat_folder_invite_link", &result->viaChatFolderInviteLink);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberUpdated) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("from", object->from);
        json.put("date", object->date);
        json.put("old_chat_member", object->oldChatMember);
        json.put("new_chat_member", object->newChatMember);
        json.put("invite_link", object->inviteLink);
        json.put("via_join_request", object->viaJoinRequest);
        json.put("via_chat_folder_invite_link", object->viaChatFolderInviteLink);
    }
    return json;
}

} // namespace TgBot
