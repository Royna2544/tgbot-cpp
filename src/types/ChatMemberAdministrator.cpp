#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatMemberAdministrator) {
    auto result = std::make_shared<ChatMemberAdministrator>();
    parse(data, "status", &result->status);
    result->user = parseRequired<User>(data, "user");
    parse(data, "can_be_edited", &result->canBeEdited);
    parse(data, "is_anonymous", &result->isAnonymous);
    parse(data, "can_manage_chat", &result->canManageChat);
    parse(data, "can_delete_messages", &result->canDeleteMessages);
    parse(data, "can_manage_video_chats", &result->canManageVideoChats);
    parse(data, "can_restrict_members", &result->canRestrictMembers);
    parse(data, "can_promote_members", &result->canPromoteMembers);
    parse(data, "can_change_info", &result->canChangeInfo);
    parse(data, "can_invite_users", &result->canInviteUsers);
    parse(data, "can_post_stories", &result->canPostStories);
    parse(data, "can_edit_stories", &result->canEditStories);
    parse(data, "can_delete_stories", &result->canDeleteStories);
    parse(data, "can_post_messages", &result->canPostMessages);
    parse(data, "can_edit_messages", &result->canEditMessages);
    parse(data, "can_pin_messages", &result->canPinMessages);
    parse(data, "can_manage_topics", &result->canManageTopics);
    parse(data, "can_manage_direct_messages", &result->canManageDirectMessages);
    parse(data, "can_manage_tags", &result->canManageTags);
    parse(data, "custom_title", &result->customTitle);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberAdministrator) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        json.put("user", object->user);
        json.put("can_be_edited", object->canBeEdited);
        json.put("is_anonymous", object->isAnonymous);
        json.put("can_manage_chat", object->canManageChat);
        json.put("can_delete_messages", object->canDeleteMessages);
        json.put("can_manage_video_chats", object->canManageVideoChats);
        json.put("can_restrict_members", object->canRestrictMembers);
        json.put("can_promote_members", object->canPromoteMembers);
        json.put("can_change_info", object->canChangeInfo);
        json.put("can_invite_users", object->canInviteUsers);
        json.put("can_post_stories", object->canPostStories);
        json.put("can_edit_stories", object->canEditStories);
        json.put("can_delete_stories", object->canDeleteStories);
        json.put("can_post_messages", object->canPostMessages);
        json.put("can_edit_messages", object->canEditMessages);
        json.put("can_pin_messages", object->canPinMessages);
        json.put("can_manage_topics", object->canManageTopics);
        json.put("can_manage_direct_messages", object->canManageDirectMessages);
        json.put("can_manage_tags", object->canManageTags);
        json.put("custom_title", object->customTitle);
    }
    return json;
}

} // namespace TgBot
