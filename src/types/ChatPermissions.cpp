#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatPermissions) {
    auto result = std::make_shared<ChatPermissions>();
    parse(data, "can_send_messages", &result->canSendMessages);
    parse(data, "can_send_audios", &result->canSendAudios);
    parse(data, "can_send_documents", &result->canSendDocuments);
    parse(data, "can_send_photos", &result->canSendPhotos);
    parse(data, "can_send_videos", &result->canSendVideos);
    parse(data, "can_send_video_notes", &result->canSendVideoNotes);
    parse(data, "can_send_voice_notes", &result->canSendVoiceNotes);
    parse(data, "can_send_polls", &result->canSendPolls);
    parse(data, "can_send_other_messages", &result->canSendOtherMessages);
    parse(data, "can_add_web_page_previews", &result->canAddWebPagePreviews);
    parse(data, "can_edit_tag", &result->canEditTag);
    parse(data, "can_change_info", &result->canChangeInfo);
    parse(data, "can_invite_users", &result->canInviteUsers);
    parse(data, "can_pin_messages", &result->canPinMessages);
    parse(data, "can_manage_topics", &result->canManageTopics);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatPermissions) {
    JsonWrapper json;
    if (object) {
        json.put("can_send_messages", object->canSendMessages);
        json.put("can_send_audios", object->canSendAudios);
        json.put("can_send_documents", object->canSendDocuments);
        json.put("can_send_photos", object->canSendPhotos);
        json.put("can_send_videos", object->canSendVideos);
        json.put("can_send_video_notes", object->canSendVideoNotes);
        json.put("can_send_voice_notes", object->canSendVoiceNotes);
        json.put("can_send_polls", object->canSendPolls);
        json.put("can_send_other_messages", object->canSendOtherMessages);
        json.put("can_add_web_page_previews", object->canAddWebPagePreviews);
        json.put("can_edit_tag", object->canEditTag);
        json.put("can_change_info", object->canChangeInfo);
        json.put("can_invite_users", object->canInviteUsers);
        json.put("can_pin_messages", object->canPinMessages);
        json.put("can_manage_topics", object->canManageTopics);
    }
    return json;
}

} // namespace TgBot
