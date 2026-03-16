#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatFullInfo) {
    auto result = std::make_shared<ChatFullInfo>();
    parse(data, "id", &result->id);
    parse(data, "type", &result->type);
    parse(data, "title", &result->title);
    parse(data, "username", &result->username);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "is_forum", &result->isForum);
    parse(data, "is_direct_messages", &result->isDirectMessages);
    parse(data, "accent_color_id", &result->accentColorId);
    parse(data, "max_reaction_count", &result->maxReactionCount);
    result->photo = parse<ChatPhoto>(data, "photo");
    result->activeUsernames = parsePrimitiveArray<std::string>(data, "active_usernames");
    result->birthdate = parse<Birthdate>(data, "birthdate");
    result->businessIntro = parse<BusinessIntro>(data, "business_intro");
    result->businessLocation = parse<BusinessLocation>(data, "business_location");
    result->businessOpeningHours = parse<BusinessOpeningHours>(data, "business_opening_hours");
    result->personalChat = parse<Chat>(data, "personal_chat");
    result->parentChat = parse<Chat>(data, "parent_chat");
    result->availableReactions = parseArray<ReactionType>(data, "available_reactions");
    parse(data, "background_custom_emoji_id", &result->backgroundCustomEmojiId);
    parse(data, "profile_accent_color_id", &result->profileAccentColorId);
    parse(data, "profile_background_custom_emoji_id", &result->profileBackgroundCustomEmojiId);
    parse(data, "emoji_status_custom_emoji_id", &result->emojiStatusCustomEmojiId);
    parse(data, "emoji_status_expiration_date", &result->emojiStatusExpirationDate);
    parse(data, "bio", &result->bio);
    parse(data, "has_private_forwards", &result->hasPrivateForwards);
    parse(data, "has_restricted_voice_and_video_messages", &result->hasRestrictedVoiceAndVideoMessages);
    parse(data, "join_to_send_messages", &result->joinToSendMessages);
    parse(data, "join_by_request", &result->joinByRequest);
    parse(data, "description", &result->description);
    parse(data, "invite_link", &result->inviteLink);
    result->pinnedMessage = parse<Message>(data, "pinned_message");
    result->permissions = parse<ChatPermissions>(data, "permissions");
    result->acceptedGiftTypes = parseRequired<AcceptedGiftTypes>(data, "accepted_gift_types");
    parse(data, "can_send_paid_media", &result->canSendPaidMedia);
    parse(data, "slow_mode_delay", &result->slowModeDelay);
    parse(data, "unrestrict_boost_count", &result->unrestrictBoostCount);
    parse(data, "message_auto_delete_time", &result->messageAutoDeleteTime);
    parse(data, "has_aggressive_anti_spam_enabled", &result->hasAggressiveAntiSpamEnabled);
    parse(data, "has_hidden_members", &result->hasHiddenMembers);
    parse(data, "has_protected_content", &result->hasProtectedContent);
    parse(data, "has_visible_history", &result->hasVisibleHistory);
    parse(data, "sticker_set_name", &result->stickerSetName);
    parse(data, "can_set_sticker_set", &result->canSetStickerSet);
    parse(data, "custom_emoji_sticker_set_name", &result->customEmojiStickerSetName);
    parse(data, "linked_chat_id", &result->linkedChatId);
    result->location = parse<ChatLocation>(data, "location");
    result->rating = parse<UserRating>(data, "rating");
    result->firstProfileAudio = parse<Audio>(data, "first_profile_audio");
    result->uniqueGiftColors = parse<UniqueGiftColors>(data, "unique_gift_colors");
    parse(data, "paid_message_star_count", &result->paidMessageStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatFullInfo) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("type", object->type);
        json.put("title", object->title);
        json.put("username", object->username);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("is_forum", object->isForum);
        json.put("is_direct_messages", object->isDirectMessages);
        json.put("accent_color_id", object->accentColorId);
        json.put("max_reaction_count", object->maxReactionCount);
        json.put("photo", object->photo);
        json.put("active_usernames", object->activeUsernames);
        json.put("birthdate", object->birthdate);
        json.put("business_intro", object->businessIntro);
        json.put("business_location", object->businessLocation);
        json.put("business_opening_hours", object->businessOpeningHours);
        json.put("personal_chat", object->personalChat);
        json.put("parent_chat", object->parentChat);
        json.put("available_reactions", object->availableReactions);
        json.put("background_custom_emoji_id", object->backgroundCustomEmojiId);
        json.put("profile_accent_color_id", object->profileAccentColorId);
        json.put("profile_background_custom_emoji_id", object->profileBackgroundCustomEmojiId);
        json.put("emoji_status_custom_emoji_id", object->emojiStatusCustomEmojiId);
        json.put("emoji_status_expiration_date", object->emojiStatusExpirationDate);
        json.put("bio", object->bio);
        json.put("has_private_forwards", object->hasPrivateForwards);
        json.put("has_restricted_voice_and_video_messages", object->hasRestrictedVoiceAndVideoMessages);
        json.put("join_to_send_messages", object->joinToSendMessages);
        json.put("join_by_request", object->joinByRequest);
        json.put("description", object->description);
        json.put("invite_link", object->inviteLink);
        json.put("pinned_message", object->pinnedMessage);
        json.put("permissions", object->permissions);
        json.put("accepted_gift_types", object->acceptedGiftTypes);
        json.put("can_send_paid_media", object->canSendPaidMedia);
        json.put("slow_mode_delay", object->slowModeDelay);
        json.put("unrestrict_boost_count", object->unrestrictBoostCount);
        json.put("message_auto_delete_time", object->messageAutoDeleteTime);
        json.put("has_aggressive_anti_spam_enabled", object->hasAggressiveAntiSpamEnabled);
        json.put("has_hidden_members", object->hasHiddenMembers);
        json.put("has_protected_content", object->hasProtectedContent);
        json.put("has_visible_history", object->hasVisibleHistory);
        json.put("sticker_set_name", object->stickerSetName);
        json.put("can_set_sticker_set", object->canSetStickerSet);
        json.put("custom_emoji_sticker_set_name", object->customEmojiStickerSetName);
        json.put("linked_chat_id", object->linkedChatId);
        json.put("location", object->location);
        json.put("rating", object->rating);
        json.put("first_profile_audio", object->firstProfileAudio);
        json.put("unique_gift_colors", object->uniqueGiftColors);
        json.put("paid_message_star_count", object->paidMessageStarCount);
    }
    return json;
}

} // namespace TgBot
