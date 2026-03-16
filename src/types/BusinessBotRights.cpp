#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BusinessBotRights) {
    auto result = std::make_shared<BusinessBotRights>();
    parse(data, "can_reply", &result->canReply);
    parse(data, "can_read_messages", &result->canReadMessages);
    parse(data, "can_delete_sent_messages", &result->canDeleteSentMessages);
    parse(data, "can_delete_all_messages", &result->canDeleteAllMessages);
    parse(data, "can_edit_name", &result->canEditName);
    parse(data, "can_edit_bio", &result->canEditBio);
    parse(data, "can_edit_profile_photo", &result->canEditProfilePhoto);
    parse(data, "can_edit_username", &result->canEditUsername);
    parse(data, "can_change_gift_settings", &result->canChangeGiftSettings);
    parse(data, "can_view_gifts_and_stars", &result->canViewGiftsAndStars);
    parse(data, "can_convert_gifts_to_stars", &result->canConvertGiftsToStars);
    parse(data, "can_transfer_and_upgrade_gifts", &result->canTransferAndUpgradeGifts);
    parse(data, "can_transfer_stars", &result->canTransferStars);
    parse(data, "can_manage_stories", &result->canManageStories);
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessBotRights) {
    JsonWrapper json;
    if (object) {
        json.put("can_reply", object->canReply);
        json.put("can_read_messages", object->canReadMessages);
        json.put("can_delete_sent_messages", object->canDeleteSentMessages);
        json.put("can_delete_all_messages", object->canDeleteAllMessages);
        json.put("can_edit_name", object->canEditName);
        json.put("can_edit_bio", object->canEditBio);
        json.put("can_edit_profile_photo", object->canEditProfilePhoto);
        json.put("can_edit_username", object->canEditUsername);
        json.put("can_change_gift_settings", object->canChangeGiftSettings);
        json.put("can_view_gifts_and_stars", object->canViewGiftsAndStars);
        json.put("can_convert_gifts_to_stars", object->canConvertGiftsToStars);
        json.put("can_transfer_and_upgrade_gifts", object->canTransferAndUpgradeGifts);
        json.put("can_transfer_stars", object->canTransferStars);
        json.put("can_manage_stories", object->canManageStories);
    }
    return json;
}

} // namespace TgBot
