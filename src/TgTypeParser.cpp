#include <json/value.h>
#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>

#include <cstdint>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>

namespace TgBot {

// T should be instance of std::shared_ptr.
template <typename T>
std::shared_ptr<T> parse(const Json::Value &data, const std::string &key) {
    if (!data.isMember(key)) {
        return nullptr;
    }
    return parse<T>(data[key]);
}

TgException invalidType(const std::string_view name,
                        const std::string_view type) {
    std::stringstream ss;
    ss << "Invalid type for " << name << ": " << type;
    return TgException(ss.str(), TgException::ErrorCode::Internal);
}

struct JsonWrapper {
    JsonWrapper() = default;

    template <typename T, std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, T value) {
        data_[key.data()] = std::move(value);
    }
    template <typename T, std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::optional<T> value) {
        if (!value) {
            return; // Skip empty optional
        }
        data_[key.data()] = *value;
    }
    // Overload for JSON::Value to avoid null values.
    void put(const std::string_view key, Json::Value value) {
        if (!value.empty()) {
            data_[key.data()] = std::move(value);
        }
    }

    static void merge(Json::Value &thiz, const Json::Value &other) {
        if (!thiz.isObject() || !other.isObject()) {
            return;
        }

        for (const auto &key : other.getMemberNames()) {
            if (thiz[key].isObject()) {
                merge(thiz[key], other[key]);
            } else {
                thiz[key] = other[key];
            }
        }
    }

    void operator+=(const Json::Value &other) { merge(data_, other); }

    JsonWrapper &operator=(Json::Value &&other) {
        data_ = std::forward<decltype(other)>(other);
        return *this;
    }
    operator Json::Value() const { return data_; }

   private:
    Json::Value data_;
};

template <typename T, std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
T parsePrimitive(const Json::Value &data, const std::string &key, T def = {}) {
    if (data[key].is<T>()) {
        return data[key].as<T>();
    }
    return def;
}

// Pre-defined parsing functions for known types.
DECLARE_PARSER_FROM_JSON(Message) {
    auto result = std::make_shared<Message>();
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    result->messageThreadId =
        parsePrimitive<std::int32_t>(data, "message_thread_id");
    result->from = parse<User>(data, "from");
    result->senderChat = parse<Chat>(data, "sender_chat");
    result->senderBoostCount =
        parsePrimitive<std::int32_t>(data, "sender_boost_count");
    result->senderBusinessBot = parse<User>(data, "sender_business_bot");
    result->date = parsePrimitive<std::uint32_t>(data, "date");
    result->businessConnectionId =
        parsePrimitive<std::string>(data, "business_connection_id");
    result->chat = parse<Chat>(data, "chat");
    result->forwardOrigin = parse<MessageOrigin>(data, "forward_origin");
    result->isTopicMessage = parsePrimitive<bool>(data, "is_topic_message");
    result->isAutomaticForward =
        parsePrimitive<bool>(data, "is_automatic_forward");
    result->replyToMessage = parse<Message>(data, "reply_to_message");
    result->externalReply = parse<ExternalReplyInfo>(data, "external_reply");
    result->quote = parse<TextQuote>(data, "quote");
    result->replyToStory = parse<Story>(data, "reply_to_story");
    result->viaBot = parse<User>(data, "via_bot");
    result->editDate = parsePrimitive<std::uint32_t>(data, "edit_date");
    result->hasProtectedContent =
        parsePrimitive<bool>(data, "has_protected_content");
    result->isFromOffline = parsePrimitive<bool>(data, "is_from_offline");
    result->mediaGroupId = parsePrimitive<std::string>(data, "media_group_id");
    result->authorSignature =
        parsePrimitive<std::string>(data, "author_signature");
    result->text = parsePrimitive<std::string>(data, "text");
    result->entities = parseArray<MessageEntity>(data, "entities");
    result->linkPreviewOptions =
        parse<LinkPreviewOptions>(data, "link_preview_options");
    result->animation = parse<Animation>(data, "animation");
    result->audio = parse<Audio>(data, "audio");
    result->document = parse<Document>(data, "document");
    result->photo = parseArray<PhotoSize>(data, "photo");
    result->sticker = parse<Sticker>(data, "sticker");
    result->story = parse<Story>(data, "story");
    result->video = parse<Video>(data, "video");
    result->videoNote = parse<VideoNote>(data, "video_note");
    result->voice = parse<Voice>(data, "voice");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->hasMediaSpoiler = parsePrimitive<bool>(data, "has_media_spoiler");
    result->contact = parse<Contact>(data, "contact");
    result->dice = parse<Dice>(data, "dice");
    result->game = parse<Game>(data, "game");
    result->poll = parse<Poll>(data, "poll");
    result->venue = parse<Venue>(data, "venue");
    result->location = parse<Location>(data, "location");
    result->newChatMembers = parseArray<User>(data, "new_chat_members");
    result->leftChatMember = parse<User>(data, "left_chat_member");
    result->newChatTitle = parsePrimitive<std::string>(data, "new_chat_title");
    result->newChatPhoto = parseArray<PhotoSize>(data, "new_chat_photo");
    result->deleteChatPhoto = parsePrimitive<bool>(data, "delete_chat_photo");
    result->groupChatCreated = parsePrimitive<bool>(data, "group_chat_created");
    result->supergroupChatCreated =
        parsePrimitive<bool>(data, "supergroup_chat_created");
    result->channelChatCreated =
        parsePrimitive<bool>(data, "channel_chat_created");
    result->messageAutoDeleteTimerChanged =
        parse<MessageAutoDeleteTimerChanged>(
            data, "message_auto_delete_timer_changed");
    result->migrateToChatId =
        parsePrimitive<std::int64_t>(data, "migrate_to_chat_id");
    result->migrateFromChatId =
        parsePrimitive<std::int64_t>(data, "migrate_from_chat_id");
    result->pinnedMessage = parse<Message>(data, "pinned_message");
    result->invoice = parse<Invoice>(data, "invoice");
    result->successfulPayment =
        parse<SuccessfulPayment>(data, "successful_payment");
    result->usersShared = parse<UsersShared>(data, "users_shared");
    result->chatShared = parse<ChatShared>(data, "chat_shared");
    result->connectedWebsite =
        parsePrimitive<std::string>(data, "connected_website");
    result->writeAccessAllowed =
        parse<WriteAccessAllowed>(data, "write_access_allowed");
    result->passportData = parse<PassportData>(data, "passport_data");
    result->proximityAlertTriggered =
        parse<ProximityAlertTriggered>(data, "proximity_alert_triggered");
    result->boostAdded = parse<ChatBoostAdded>(data, "boost_added");
    result->forumTopicCreated =
        parse<ForumTopicCreated>(data, "forum_topic_created");
    result->forumTopicEdited =
        parse<ForumTopicEdited>(data, "forum_topic_edited");
    result->forumTopicClosed =
        parse<ForumTopicClosed>(data, "forum_topic_closed");
    result->forumTopicReopened =
        parse<ForumTopicReopened>(data, "forum_topic_reopened");
    result->generalForumTopicHidden =
        parse<GeneralForumTopicHidden>(data, "general_forum_topic_hidden");
    result->generalForumTopicUnhidden =
        parse<GeneralForumTopicUnhidden>(data, "general_forum_topic_unhidden");
    result->giveawayCreated = parse<GiveawayCreated>(data, "giveaway_created");
    result->giveaway = parse<Giveaway>(data, "giveaway");
    result->giveawayWinners = parse<GiveawayWinners>(data, "giveaway_winners");
    result->giveawayCompleted =
        parse<GiveawayCompleted>(data, "giveaway_completed");
    result->videoChatScheduled =
        parse<VideoChatScheduled>(data, "video_chat_scheduled");
    result->videoChatStarted =
        parse<VideoChatStarted>(data, "video_chat_started");
    result->videoChatEnded = parse<VideoChatEnded>(data, "video_chat_ended");
    result->videoChatParticipantsInvited = parse<VideoChatParticipantsInvited>(
        data, "video_chat_participants_invited");
    result->webAppData = parse<WebAppData>(data, "web_app_data");
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    return result;
}

DECLARE_PARSER_TO_JSON(Message) {
    JsonWrapper json;

    if (object) {
        // Simple fields
        json.put("message_id", object->messageId);
        json.put("message_thread_id", object->messageThreadId);
        json.put("from", put(object->from));
        json.put("sender_chat", put(object->senderChat));
        json.put("sender_boost_count", object->senderBoostCount);
        json.put("sender_business_bot", put(object->senderBusinessBot));
        json.put("date", object->date);
        json.put("business_connection_id", object->businessConnectionId);
        json.put("chat", put(object->chat));
        json.put("forward_origin", put(object->forwardOrigin));
        json.put("is_topic_message", object->isTopicMessage);
        json.put("is_automatic_forward", object->isAutomaticForward);
        json.put("reply_to_message", put(object->replyToMessage));
        json.put("external_reply", put(object->externalReply));
        json.put("quote", put(object->quote));
        json.put("reply_to_story", put(object->replyToStory));
        json.put("via_bot", put(object->viaBot));
        json.put("edit_date", object->editDate);
        json.put("has_protected_content", object->hasProtectedContent);
        json.put("is_from_offline", object->isFromOffline);
        json.put("media_group_id", object->mediaGroupId);
        json.put("author_signature", object->authorSignature);
        json.put("text", object->text);
        json.put("entities", put(object->entities));
        json.put("link_preview_options", put(object->linkPreviewOptions));
        json.put("animation", put(object->animation));
        json.put("audio", put(object->audio));
        json.put("document", put(object->document));
        json.put("photo", put(object->photo));
        json.put("sticker", put(object->sticker));
        json.put("story", put(object->story));
        json.put("video", put(object->video));
        json.put("video_note", put(object->videoNote));
        json.put("voice", put(object->voice));
        json.put("caption", object->caption);
        json.put("caption_entities", put(object->captionEntities));
        json.put("has_media_spoiler", object->hasMediaSpoiler);
        json.put("contact", put(object->contact));
        json.put("dice", put(object->dice));
        json.put("game", put(object->game));
        json.put("poll", put(object->poll));
        json.put("venue", put(object->venue));
        json.put("location", put(object->location));
        json.put("new_chat_members", put(object->newChatMembers));
        json.put("left_chat_member", put(object->leftChatMember));
        json.put("new_chat_title", object->newChatTitle);
        json.put("new_chat_photo", put(object->newChatPhoto));
        json.put("delete_chat_photo", object->deleteChatPhoto);
        json.put("group_chat_created", object->groupChatCreated);
        json.put("supergroup_chat_created", object->supergroupChatCreated);
        json.put("channel_chat_created", object->channelChatCreated);
        json.put("message_auto_delete_timer_changed",
                 put(object->messageAutoDeleteTimerChanged));
        json.put("migrate_to_chat_id", object->migrateToChatId);
        json.put("migrate_from_chat_id", object->migrateFromChatId);
        json.put("pinned_message", put(object->pinnedMessage));
        json.put("invoice", put(object->invoice));
        json.put("successful_payment", put(object->successfulPayment));
        json.put("users_shared", put(object->usersShared));
        json.put("chat_shared", put(object->chatShared));
        json.put("connected_website", object->connectedWebsite);
        json.put("write_access_allowed", put(object->writeAccessAllowed));
        json.put("passport_data", put(object->passportData));
        json.put("proximity_alert_triggered",
                 put(object->proximityAlertTriggered));
        json.put("boost_added", put(object->boostAdded));
        json.put("forum_topic_created", put(object->forumTopicCreated));
        json.put("forum_topic_edited", put(object->forumTopicEdited));
        json.put("forum_topic_closed", put(object->forumTopicClosed));
        json.put("forum_topic_reopened", put(object->forumTopicReopened));
        json.put("general_forum_topic_hidden",
                 put(object->generalForumTopicHidden));
        json.put("general_forum_topic_unhidden",
                 put(object->generalForumTopicUnhidden));
        json.put("giveaway_created", put(object->giveawayCreated));
        json.put("giveaway", put(object->giveaway));
        json.put("giveaway_winners", put(object->giveawayWinners));
        json.put("giveaway_completed", put(object->giveawayCompleted));
        json.put("video_chat_scheduled", put(object->videoChatScheduled));
        json.put("video_chat_started", put(object->videoChatStarted));
        json.put("video_chat_ended", put(object->videoChatEnded));
        json.put("video_chat_participants_invited",
                 put(object->videoChatParticipantsInvited));
        json.put("web_app_data", put(object->webAppData));
        json.put("reply_markup", put(object->replyMarkup));
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Update) {
    auto result(std::make_shared<Update>());
    result->updateId = parsePrimitive<std::int32_t>(data, "update_id");
    result->message = parse<Message>(data, "message");
    result->editedMessage = parse<Message>(data, "edited_message");
    result->channelPost = parse<Message>(data, "channel_post");
    result->editedChannelPost = parse<Message>(data, "edited_channel_post");
    result->businessConnection =
        parse<BusinessConnection>(data, "business_connection");
    result->businessMessage = parse<Message>(data, "business_message");
    result->editedBusinessMessage =
        parse<Message>(data, "edited_business_message");
    result->deletedBusinessMessages =
        parse<BusinessMessagesDeleted>(data, "deleted_business_messages");
    result->messageReaction =
        parse<MessageReactionUpdated>(data, "message_reaction");
    result->messageReactionCount =
        parse<MessageReactionCountUpdated>(data, "message_reaction_count");
    result->inlineQuery = parse<InlineQuery>(data, "inline_query");
    result->chosenInlineResult =
        parse<ChosenInlineResult>(data, "chosen_inline_result");
    result->callbackQuery = parse<CallbackQuery>(data, "callback_query");
    result->shippingQuery = parse<ShippingQuery>(data, "shipping_query");
    result->preCheckoutQuery =
        parse<PreCheckoutQuery>(data, "pre_checkout_query");
    result->poll = parse<Poll>(data, "poll");
    result->pollAnswer = parse<PollAnswer>(data, "poll_answer");
    result->myChatMember = parse<ChatMemberUpdated>(data, "my_chat_member");
    result->chatMember = parse<ChatMemberUpdated>(data, "chat_member");
    result->chatJoinRequest = parse<ChatJoinRequest>(data, "chat_join_request");
    result->chatBoost = parse<ChatBoostUpdated>(data, "chat_boost");
    result->removedChatBoost =
        parse<ChatBoostRemoved>(data, "removed_chat_boost");
    return result;
}

DECLARE_PARSER_TO_JSON(Update) {
    JsonWrapper json;
    if (object) {
        json.put("update_id", object->updateId);
        json.put("message", put(object->message));
        json.put("edited_message", put(object->editedMessage));
        json.put("channel_post", put(object->channelPost));
        json.put("edited_channel_post", put(object->editedChannelPost));
        json.put("business_connection", put(object->businessConnection));
        json.put("business_message", put(object->businessMessage));
        json.put("edited_business_message", put(object->editedBusinessMessage));
        json.put("deleted_business_messages",
                 put(object->deletedBusinessMessages));
        json.put("message_reaction", put(object->messageReaction));
        json.put("message_reaction_count", put(object->messageReactionCount));
        json.put("inline_query", put(object->inlineQuery));
        json.put("chosen_inline_result", put(object->chosenInlineResult));
        json.put("callback_query", put(object->callbackQuery));
        json.put("shipping_query", put(object->shippingQuery));
        json.put("pre_checkout_query", put(object->preCheckoutQuery));
        json.put("poll", put(object->poll));
        json.put("poll_answer", put(object->pollAnswer));
        json.put("my_chat_member", put(object->myChatMember));
        json.put("chat_member", put(object->chatMember));
        json.put("chat_join_request", put(object->chatJoinRequest));
        json.put("chat_boost", put(object->chatBoost));
        json.put("removed_chat_boost", put(object->removedChatBoost));
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(WebhookInfo) {
    auto result(std::make_shared<WebhookInfo>());
    result->url = parsePrimitive<std::string>(data, "url");
    result->hasCustomCertificate =
        parsePrimitive<bool>(data, "has_custom_certificate");
    result->pendingUpdateCount =
        parsePrimitive<std::int32_t>(data, "pending_update_count");
    result->ipAddress = parsePrimitive<std::string>(data, "ip_address");
    result->lastErrorDate =
        parsePrimitive<std::int32_t>(data, "last_error_date");
    result->lastErrorMessage =
        parsePrimitive<std::string>(data, "last_error_message");
    result->lastSynchronizationErrorDate =
        parsePrimitive<std::int32_t>(data, "last_synchronization_error_date");
    result->maxConnections =
        parsePrimitive<std::int32_t>(data, "max_connections");
    result->allowedUpdates =
        parsePrimitiveArray<std::string>(data, "allowed_updates");
    return result;
}

DECLARE_PARSER_TO_JSON(WebhookInfo) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
        json.put("has_custom_certificate", object->hasCustomCertificate);
        json.put("pending_update_count", object->pendingUpdateCount);
        json.put("ip_address", object->ipAddress);
        json.put("last_error_date", object->lastErrorDate);
        json.put("last_error_message", object->lastErrorMessage);
        json.put("last_synchronization_error_date",
                 object->lastSynchronizationErrorDate);
        json.put("max_connections", object->maxConnections);

        json.put("allowed_updates", put(object->allowedUpdates));
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(User) {
    auto result(std::make_shared<User>());
    result->id = parsePrimitive<std::int64_t>(data, "id");
    result->isBot = parsePrimitive<bool>(data, "is_bot");
    result->firstName = parsePrimitive<std::string>(data, "first_name");
    result->lastName = parsePrimitive<std::string>(data, "last_name");
    result->username = parsePrimitive<std::string>(data, "username");
    result->languageCode = parsePrimitive<std::string>(data, "language_code");
    result->isPremium = parsePrimitive<bool>(data, "is_premium");
    result->addedToAttachmentMenu =
        parsePrimitive<bool>(data, "added_to_attachment_menu");
    result->canJoinGroups = parsePrimitive<bool>(data, "can_join_groups");
    result->canReadAllGroupMessages =
        parsePrimitive<bool>(data, "can_read_all_group_messages");
    result->supportsInlineQueries =
        parsePrimitive<bool>(data, "supports_inline_queries");
    result->canConnectToBusiness =
        parsePrimitive<bool>(data, "can_connect_to_business");
    return result;
}

DECLARE_PARSER_TO_JSON(User) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("is_bot", object->isBot);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("username", object->username);
        json.put("language_code", object->languageCode);
        json.put("is_premium", object->isPremium);
        json.put("added_to_attachment_menu", object->addedToAttachmentMenu);
        json.put("can_join_groups", object->canJoinGroups);
        json.put("can_read_all_group_messages",
                 object->canReadAllGroupMessages);
        json.put("supports_inline_queries", object->supportsInlineQueries);
        json.put("can_connect_to_business", object->canConnectToBusiness);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Chat) {
    auto result(std::make_shared<Chat>());
    result->id = parsePrimitive<std::int64_t>(data, "id");
    std::string type = parsePrimitive<std::string>(data, "type");
    if (type == "private") {
        result->type = Chat::Type::Private;
    } else if (type == "group") {
        result->type = Chat::Type::Group;
    } else if (type == "supergroup") {
        result->type = Chat::Type::Supergroup;
    } else if (type == "channel") {
        result->type = Chat::Type::Channel;
    }
    result->title = parsePrimitive<std::string>(data, "title");
    result->username = parsePrimitive<std::string>(data, "username");
    result->firstName = parsePrimitive<std::string>(data, "first_name");
    result->lastName = parsePrimitive<std::string>(data, "last_name");
    result->isForum = parsePrimitive<bool>(data, "is_forum");
    result->photo = parse<ChatPhoto>(data, "photo");
    result->activeUsernames =
        parsePrimitiveArray<std::string>(data, "active_usernames");
    result->birthdate = parse<Birthdate>(data, "birthdate");
    result->businessIntro = parse<BusinessIntro>(data, "business_intro");
    result->businessLocation =
        parse<BusinessLocation>(data, "business_location");
    result->businessOpeningHours =
        parse<BusinessOpeningHours>(data, "business_opening_hours");
    result->personalChat = parse<Chat>(data, "personal_chat");
    result->availableReactions =
        parseArray<ReactionType>(data, "available_reactions");
    result->accentColorId =
        parsePrimitive<std::int32_t>(data, "accent_color_id");
    result->backgroundCustomEmojiId =
        parsePrimitive<std::string>(data, "background_custom_emoji_id");
    result->profileAccentColorId =
        parsePrimitive<std::int32_t>(data, "profile_accent_color_id");
    result->profileBackgroundCustomEmojiId =
        parsePrimitive<std::string>(data, "profile_background_custom_emoji_id");
    result->emojiStatusCustomEmojiId =
        parsePrimitive<std::string>(data, "emoji_status_custom_emoji_id");
    result->emojiStatusExpirationDate =
        parsePrimitive<std::uint32_t>(data, "emoji_status_expiration_date");
    result->bio = parsePrimitive<std::string>(data, "bio");
    result->hasPrivateForwards =
        parsePrimitive<bool>(data, "has_private_forwards");
    result->hasRestrictedVoiceAndVideoMessages =
        parsePrimitive<bool>(data, "has_restricted_voice_and_video_messages");
    result->joinToSendMessages =
        parsePrimitive<bool>(data, "join_to_send_messages");
    result->joinByRequest = parsePrimitive<bool>(data, "join_by_request");
    result->description = parsePrimitive<std::string>(data, "description");
    result->inviteLink = parsePrimitive<std::string>(data, "invite_link");
    result->pinnedMessage = parse<Message>(data, "pinned_message");
    result->permissions = parse<ChatPermissions>(data, "permissions");
    result->slowModeDelay =
        parsePrimitive<std::int32_t>(data, "slow_mode_delay");
    result->unrestrictBoostCount =
        parsePrimitive<std::int32_t>(data, "unrestrict_boost_count");
    result->messageAutoDeleteTime =
        parsePrimitive<std::int32_t>(data, "message_auto_delete_time");
    result->hasAggressiveAntiSpamEnabled =
        parsePrimitive<bool>(data, "has_aggressive_anti_spam_enabled");
    result->hasHiddenMembers = parsePrimitive<bool>(data, "has_hidden_members");
    result->hasProtectedContent =
        parsePrimitive<bool>(data, "has_protected_content");
    result->hasVisibleHistory =
        parsePrimitive<bool>(data, "has_visible_history");
    result->stickerSetName =
        parsePrimitive<std::string>(data, "sticker_set_name");
    result->canSetStickerSet =
        parsePrimitive<bool>(data, "can_set_sticker_set");
    result->customEmojiStickerSetName =
        parsePrimitive<std::string>(data, "custom_emoji_sticker_set_name");
    result->linkedChatId = parsePrimitive<std::int64_t>(data, "linked_chat_id");
    result->location = parse<ChatLocation>(data, "location");
    return result;
}

DECLARE_PARSER_TO_JSON(Chat) {
    JsonWrapper json;

    if (object) {
        json.put("id", object->id);

        switch (object->type) {
            case Chat::Type::Private:
                json.put("type", "private");
                break;
            case Chat::Type::Group:
                json.put("type", "group");
                break;
            case Chat::Type::Supergroup:
                json.put("type", "supergroup");
                break;
            case Chat::Type::Channel:
                json.put("type", "channel");
                break;
        }

        json.put("title", object->title);
        json.put("username", object->username);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("is_forum", object->isForum);
        json.put("photo", put(object->photo));
        json.put("active_usernames", put(object->activeUsernames));
        json.put("birthdate", put(object->birthdate));
        json.put("business_intro", put(object->businessIntro));
        json.put("business_location", put(object->businessLocation));
        json.put("business_opening_hours", put(object->businessOpeningHours));
        json.put("personal_chat", put(object->personalChat));

        json.put("available_reactions", put(object->availableReactions));

        json.put("accent_color_id", object->accentColorId);
        json.put("background_custom_emoji_id", object->backgroundCustomEmojiId);
        json.put("profile_accent_color_id", object->profileAccentColorId);
        json.put("profile_background_custom_emoji_id",
                 object->profileBackgroundCustomEmojiId);
        json.put("emoji_status_custom_emoji_id",
                 object->emojiStatusCustomEmojiId);
        json.put("emoji_status_expiration_date",
                 object->emojiStatusExpirationDate);
        json.put("bio", object->bio);
        json.put("has_private_forwards", object->hasPrivateForwards);
        json.put("has_restricted_voice_and_video_messages",
                 object->hasRestrictedVoiceAndVideoMessages);
        json.put("join_to_send_messages", object->joinToSendMessages);
        json.put("join_by_request", object->joinByRequest);
        json.put("description", object->description);
        json.put("invite_link", object->inviteLink);
        json.put("pinned_message", put(object->pinnedMessage));
        json.put("permissions", put(object->permissions));
        json.put("slow_mode_delay", object->slowModeDelay);
        json.put("unrestrict_boost_count", object->unrestrictBoostCount);
        json.put("message_auto_delete_time", object->messageAutoDeleteTime);
        json.put("has_aggressive_anti_spam_enabled",
                 object->hasAggressiveAntiSpamEnabled);
        json.put("has_hidden_members", object->hasHiddenMembers);
        json.put("has_protected_content", object->hasProtectedContent);
        json.put("has_visible_history", object->hasVisibleHistory);
        json.put("sticker_set_name", object->stickerSetName);
        json.put("can_set_sticker_set", object->canSetStickerSet);
        json.put("custom_emoji_sticker_set_name",
                 object->customEmojiStickerSetName);
        json.put("linked_chat_id", object->linkedChatId);
        json.put("location", put(object->location));
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(MessageId) {
    auto result(std::make_shared<MessageId>());
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageId) {
    JsonWrapper json;

    if (object) {
        json.put("message_id", object->messageId);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(InaccessibleMessage) {
    auto result(std::make_shared<InaccessibleMessage>());
    result->chat = parse<Chat>(data, "chat");
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    result->date = (std::uint8_t)parsePrimitive<std::int32_t>(data, "date");
    return result;
}

DECLARE_PARSER_TO_JSON(InaccessibleMessage) {
    JsonWrapper json;

    if (object) {
        json.put("message_id", object->messageId);
        json.put("date", object->date);
        json.put("chat", put(object->chat));
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(MessageEntity) {
    auto result(std::make_shared<MessageEntity>());
    std::string type = parsePrimitive<std::string>(data, "type");
    if (type == "mention") {
        result->type = MessageEntity::Type::Mention;
    } else if (type == "hashtag") {
        result->type = MessageEntity::Type::Hashtag;
    } else if (type == "cashtag") {
        result->type = MessageEntity::Type::Cashtag;
    } else if (type == "bot_command") {
        result->type = MessageEntity::Type::BotCommand;
    } else if (type == "url") {
        result->type = MessageEntity::Type::Url;
    } else if (type == "email") {
        result->type = MessageEntity::Type::Email;
    } else if (type == "phone_number") {
        result->type = MessageEntity::Type::PhoneNumber;
    } else if (type == "bold") {
        result->type = MessageEntity::Type::Bold;
    } else if (type == "italic") {
        result->type = MessageEntity::Type::Italic;
    } else if (type == "underline") {
        result->type = MessageEntity::Type::Underline;
    } else if (type == "strikethrough") {
        result->type = MessageEntity::Type::Strikethrough;
    } else if (type == "spoiler") {
        result->type = MessageEntity::Type::Spoiler;
    } else if (type == "blockquote") {
        result->type = MessageEntity::Type::Blockquote;
    } else if (type == "code") {
        result->type = MessageEntity::Type::Code;
    } else if (type == "pre") {
        result->type = MessageEntity::Type::Pre;
    } else if (type == "text_link") {
        result->type = MessageEntity::Type::TextLink;
    } else if (type == "text_mention") {
        result->type = MessageEntity::Type::TextMention;
    } else if (type == "custom_emoji") {
        result->type = MessageEntity::Type::CustomEmoji;
    }
    result->offset = parsePrimitive<std::int32_t>(data, "offset");
    result->length = parsePrimitive<std::int32_t>(data, "length");
    result->url = parsePrimitive<std::string>(data, "url");
    result->user = parse<User>(data, "user");
    result->language = parsePrimitive<std::string>(data, "language");
    result->customEmojiId =
        parsePrimitive<std::string>(data, "custom_emoji_id");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageEntity) {
    JsonWrapper json;

    if (object) {
        switch (object->type) {
            case MessageEntity::Type::Mention:
                json.put("type", "mention");
                break;
            case MessageEntity::Type::Hashtag:
                json.put("type", "hashtag");
                break;
            case MessageEntity::Type::Cashtag:
                json.put("type", "cashtag");
                break;
            case MessageEntity::Type::BotCommand:
                json.put("type", "bot_command");
                break;
            case MessageEntity::Type::Url:
                json.put("type", "url");
                break;
            case MessageEntity::Type::Email:
                json.put("type", "email");
                break;
            case MessageEntity::Type::PhoneNumber:
                json.put("type", "phone_number");
                break;
            case MessageEntity::Type::Bold:
                json.put("type", "bold");
                break;
            case MessageEntity::Type::Italic:
                json.put("type", "italic");
                break;
            case MessageEntity::Type::Underline:
                json.put("type", "underline");
                break;
            case MessageEntity::Type::Strikethrough:
                json.put("type", "strikethrough");
                break;
            case MessageEntity::Type::Spoiler:
                json.put("type", "spoiler");
                break;
            case MessageEntity::Type::Blockquote:
                json.put("type", "blockquote");
                break;
            case MessageEntity::Type::Code:
                json.put("type", "code");
                break;
            case MessageEntity::Type::Pre:
                json.put("type", "pre");
                break;
            case MessageEntity::Type::TextLink:
                json.put("type", "text_link");
                break;
            case MessageEntity::Type::TextMention:
                json.put("type", "text_mention");
                break;
            case MessageEntity::Type::CustomEmoji:
                json.put("type", "custom_emoji");
                break;
        }

        json.put("offset", object->offset);
        json.put("length", object->length);
        json.put("url", object->url);
        json.put("user", put(object->user));
        json.put("language", object->language);
        json.put("custom_emoji_id", object->customEmojiId);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TextQuote) {
    auto result(std::make_shared<TextQuote>());
    result->text = parsePrimitive<std::string>(data, "text");
    result->entities = parseArray<MessageEntity>(data, "entities");
    result->position = parsePrimitive<std::int32_t>(data, "position");
    result->isManual = parsePrimitive<bool>(data, "is_manual");
    return result;
}

DECLARE_PARSER_TO_JSON(TextQuote) {
    JsonWrapper json;

    if (object) {
        json.put("text", object->text);
        json.put("entities", put(object->entities));
        json.put("position", object->position);
        json.put("is_manual", object->isManual);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(ExternalReplyInfo) {
    auto result(std::make_shared<ExternalReplyInfo>());
    result->origin = parse<MessageOrigin>(data, "origin");
    result->chat = parse<Chat>(data, "chat");
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    result->linkPreviewOptions =
        parse<LinkPreviewOptions>(data, "link_preview_options");
    result->animation = parse<Animation>(data, "animation");
    result->audio = parse<Audio>(data, "audio");
    result->document = parse<Document>(data, "document");
    result->photo = parseArray<PhotoSize>(data, "photo");
    result->sticker = parse<Sticker>(data, "sticker");
    result->story = parse<Story>(data, "story");
    result->video = parse<Video>(data, "video");
    result->videoNote = parse<VideoNote>(data, "video_note");
    result->voice = parse<Voice>(data, "voice");
    result->hasMediaSpoiler = parsePrimitive<bool>(data, "has_media_spoiler");
    result->contact = parse<Contact>(data, "contact");
    result->dice = parse<Dice>(data, "dice");
    result->game = parse<Game>(data, "game");
    result->giveaway = parse<Giveaway>(data, "giveaway");
    result->giveawayWinners = parse<GiveawayWinners>(data, "giveaway_winners");
    result->invoice = parse<Invoice>(data, "invoice");
    result->location = parse<Location>(data, "location");
    result->poll = parse<Poll>(data, "poll");
    result->venue = parse<Venue>(data, "venue");
    return result;
}

DECLARE_PARSER_TO_JSON(ExternalReplyInfo) {
    JsonWrapper json;

    if (object) {
        json.put("origin", put(object->origin));
        json.put("chat", put(object->chat));
        json.put("message_id", object->messageId);
        json.put("link_preview_options", put(object->linkPreviewOptions));
        json.put("animation", put(object->animation));
        json.put("audio", put(object->audio));
        json.put("document", put(object->document));
        json.put("photo", put(object->photo));
        json.put("sticker", put(object->sticker));
        json.put("story", put(object->story));
        json.put("video", put(object->video));
        json.put("video_note", put(object->videoNote));
        json.put("voice", put(object->voice));
        json.put("has_media_spoiler", object->hasMediaSpoiler);
        json.put("contact", put(object->contact));
        json.put("dice", put(object->dice));
        json.put("game", put(object->game));
        json.put("giveaway", put(object->giveaway));
        json.put("giveaway_winners", put(object->giveawayWinners));
        json.put("invoice", put(object->invoice));
        json.put("location", put(object->location));
        json.put("poll", put(object->poll));
        json.put("venue", put(object->venue));
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(ReplyParameters) {
    auto result(std::make_shared<ReplyParameters>());
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    result->chatId = parsePrimitive<std::int64_t>(data, "chat_id");
    result->allowSendingWithoutReply =
        parsePrimitive<bool>(data, "allow_sending_without_reply");
    result->quote = parsePrimitive<std::string>(data, "quote");
    result->quoteParseMode =
        parsePrimitive<std::string>(data, "quote_parse_mode");
    result->quoteEntities = parseArray<MessageEntity>(data, "quote_entities");
    result->quotePosition =
        parsePrimitive<std::int32_t>(data, "quote_position");
    return result;
}

DECLARE_PARSER_TO_JSON(ReplyParameters) {
    JsonWrapper json;

    if (object) {
        json.put("message_id", object->messageId);
        json.put("chat_id", object->chatId);
        json.put("allow_sending_without_reply",
                 object->allowSendingWithoutReply);
        json.put("quote", object->quote);
        json.put("quote_parse_mode", object->quoteParseMode);
        json.put("quote_entities", put(object->quoteEntities));
        json.put("quote_position", object->quotePosition);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(MessageOrigin) {
    std::string type = parsePrimitive<std::string>(data, "type");
    MessageOrigin::Ptr result;

    if (type == MessageOriginUser::TYPE) {
        result = parse<MessageOriginUser>(data);
    } else if (type == MessageOriginHiddenUser::TYPE) {
        result = parse<MessageOriginHiddenUser>(data);
    } else if (type == MessageOriginChat::TYPE) {
        result = parse<MessageOriginChat>(data);
    } else if (type == MessageOriginChannel::TYPE) {
        result = parse<MessageOriginChannel>(data);
    } else {
        throw invalidType("MessageOrigin", type);
    }

    result->type = type;
    result->date = parsePrimitive<std::int32_t>(data, "date");

    return result;
}

DECLARE_PARSER_TO_JSON(MessageOrigin) {
    JsonWrapper json;

    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);

        if (object->type == MessageOriginUser::TYPE) {
            json.put("origin", put<MessageOriginUser>(object));
        } else if (object->type == MessageOriginHiddenUser::TYPE) {
            json.put("origin", put<MessageOriginHiddenUser>(object));
        } else if (object->type == MessageOriginChat::TYPE) {
            json.put("origin", put<MessageOriginChat>(object));
        } else if (object->type == MessageOriginChannel::TYPE) {
            json.put("origin", put<MessageOriginChannel>(object));
        } else {
            throw invalidType("MessageOrigin",
                                                  object->type);
        }
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(MessageOriginUser) {
    auto result(std::make_shared<MessageOriginUser>());
    result->senderUser = parse<User>(data, "sender_user");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginUser) {
    JsonWrapper json;

    if (object) {
        json.put("sender_user", put(object->senderUser));
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(MessageOriginHiddenUser) {
    auto result(std::make_shared<MessageOriginHiddenUser>());
    result->senderUserName =
        parsePrimitive<std::string>(data, "sender_user_name");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginHiddenUser) {
    JsonWrapper result;

    if (object) {
        result.put("sender_user_name", object->senderUserName);
    }
    return result;
}

DECLARE_PARSER_FROM_JSON(MessageOriginChat) {
    // NOTE: This function will be called by parseJsonAndGetMessageOrigin().
    auto result(std::make_shared<MessageOriginChat>());
    result->senderChat = parse<Chat>(data, "sender_chat");
    result->authorSignature =
        parsePrimitive<std::string>(data, "author_signature");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginChat) {
    JsonWrapper result;

    if (object) {
        result.put("sender_chat", put(object->senderChat));
        result.put("author_signature", object->authorSignature);
    }

    return result;
}

DECLARE_PARSER_FROM_JSON(MessageOriginChannel) {
    // NOTE: This function will be called by parseJsonAndGetMessageOrigin().
    auto result(std::make_shared<MessageOriginChannel>());
    result->chat = parse<Chat>(data, "chat");
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    result->authorSignature =
        parsePrimitive<std::string>(data, "author_signature");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginChannel) {
    JsonWrapper result;

    if (object) {
        result.put("chat", put(object->chat));
        result.put("message_id", object->messageId);
        result.put("author_signature", object->authorSignature);
    }

    return result;
}

DECLARE_PARSER_FROM_JSON(PhotoSize) {
    auto result(std::make_shared<PhotoSize>());
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->width = parsePrimitive<std::int32_t>(data, "width");
    result->height = parsePrimitive<std::int32_t>(data, "height");
    result->fileSize = parsePrimitive<std::int32_t>(data, "file_size");
    return result;
}

DECLARE_PARSER_TO_JSON(PhotoSize) {
    JsonWrapper json;

    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("file_size", object->fileSize);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Animation) {
    auto result(std::make_shared<Animation>());
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->width = parsePrimitive<std::int32_t>(data, "width");
    result->height = parsePrimitive<std::int32_t>(data, "height");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    result->fileName = parsePrimitive<std::string>(data, "file_name");
    result->mimeType = parsePrimitive<std::string>(data, "mime_type");
    result->fileSize = parsePrimitive<std::int64_t>(data, "file_size");
    return result;
}

DECLARE_PARSER_TO_JSON(Animation) {
    JsonWrapper json;

    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
        json.put("thumbnail", put(object->thumbnail));
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Audio) {
    auto result(std::make_shared<Audio>());
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->performer = parsePrimitive<std::string>(data, "performer");
    result->title = parsePrimitive<std::string>(data, "title");
    result->fileName = parsePrimitive<std::string>(data, "file_name");
    result->mimeType = parsePrimitive<std::string>(data, "mime_type");
    result->fileSize = parsePrimitive<std::int64_t>(data, "file_size");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    return result;
}

DECLARE_PARSER_TO_JSON(Audio) {
    JsonWrapper json;

    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("duration", object->duration);
        json.put("performer", object->performer);
        json.put("title", object->title);
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
        json.put("thumbnail", put(object->thumbnail));
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Document) {
    auto result(std::make_shared<Document>());
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    result->fileName = parsePrimitive<std::string>(data, "file_name");
    result->mimeType = parsePrimitive<std::string>(data, "mime_type");
    result->fileSize = parsePrimitive<std::int64_t>(data, "file_size");
    return result;
}

DECLARE_PARSER_TO_JSON(Document) {
    JsonWrapper json;

    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("thumbnail", put(object->thumbnail));
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Story) {
    auto result(std::make_shared<Story>());
    result->chat = parse<Chat>(data, "chat");
    result->id = parsePrimitive<std::int32_t>(data, "id");
    return result;
}

DECLARE_PARSER_TO_JSON(Story) {
    JsonWrapper json;

    if (object) {
        json.put("chat", put(object->chat));
        json.put("id", object->id);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Video) {
    auto result(std::make_shared<Video>());
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->width = parsePrimitive<std::int32_t>(data, "width");
    result->height = parsePrimitive<std::int32_t>(data, "height");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    result->fileName = parsePrimitive<std::string>(data, "file_name");
    result->mimeType = parsePrimitive<std::string>(data, "mime_type");
    result->fileSize = parsePrimitive<std::int64_t>(data, "file_size");
    return result;
}

DECLARE_PARSER_TO_JSON(Video) {
    JsonWrapper json;

    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
        json.put("thumbnail", put(object->thumbnail));
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(VideoNote) {
    auto result(std::make_shared<VideoNote>());
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->length = parsePrimitive<std::int32_t>(data, "length");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    result->fileSize = parsePrimitive<std::int32_t>(data, "file_size");
    return result;
}

DECLARE_PARSER_TO_JSON(VideoNote) {
    JsonWrapper json;

    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("length", object->length);
        json.put("duration", object->duration);
        json.put("thumbnail", put(object->thumbnail));
        json.put("file_size", object->fileSize);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Voice) {
    auto result(std::make_shared<Voice>());
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->mimeType = parsePrimitive<std::string>(data, "mime_type");
    result->fileSize = parsePrimitive<std::int64_t>(data, "file_size");
    return result;
}

DECLARE_PARSER_TO_JSON(Voice) {
    JsonWrapper json;

    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("duration", object->duration);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Contact) {
    auto result(std::make_shared<Contact>());
    result->phoneNumber = parsePrimitive<std::string>(data, "phone_number");
    result->firstName = parsePrimitive<std::string>(data, "first_name");
    result->lastName = parsePrimitive<std::string>(data, "last_name");
    result->userId = parsePrimitive<std::int64_t>(data, "user_id");
    result->vcard = parsePrimitive<std::string>(data, "vcard");
    return result;
}

DECLARE_PARSER_TO_JSON(Contact) {
    JsonWrapper json;

    if (object) {
        json.put("phone_number", object->phoneNumber);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("user_id", object->userId);
        json.put("vcard", object->vcard);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Dice) {
    auto result(std::make_shared<Dice>());
    result->emoji = parsePrimitive<std::string>(data, "emoji");
    result->value = parsePrimitive<std::int32_t>(data, "value");
    return result;
}

DECLARE_PARSER_TO_JSON(Dice) {
    JsonWrapper json;

    if (object) {
        json.put("emoji", object->emoji);
        json.put("value", object->value);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(PollOption) {
    auto result(std::make_shared<PollOption>());
    result->text = parsePrimitive<std::string>(data, "text");
    result->voterCount = parsePrimitive<std::int32_t>(data, "voter_count");
    return result;
}

DECLARE_PARSER_TO_JSON(PollOption) {
    JsonWrapper json;

    if (object) {
        json.put("text", object->text);
        json.put("voter_count", object->voterCount);
    }

    return json;
}
DECLARE_PARSER_FROM_JSON(PollAnswer) {
    auto result(std::make_shared<PollAnswer>());
    result->pollId = parsePrimitive<std::string>(data, "poll_id");
    result->voterChat = parse<Chat>(data, "voter_chat");
    result->user = parse<User>(data, "user");
    result->optionIds = parsePrimitiveArray<std::int32_t>(data, "option_ids");
    return result;
}

DECLARE_PARSER_TO_JSON(PollAnswer) {
    JsonWrapper json;
    if (object) {
        json.put("poll_id", object->pollId);
        json.put("voter_chat", put(object->voterChat));
        json.put("user", put(object->user));
        json.put("option_ids", put(object->optionIds));
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Poll) {
    auto result(std::make_shared<Poll>());
    result->id = parsePrimitive<std::string>(data, "id");
    result->question = parsePrimitive<std::string>(data, "question");
    result->options = parseArray<PollOption>(data, "options");
    result->totalVoterCount =
        parsePrimitive<std::int32_t>(data, "total_voter_count");
    result->isClosed = parsePrimitive<bool>(data, "is_closed");
    result->isAnonymous = parsePrimitive<bool>(data, "is_anonymous");
    result->type = parsePrimitive<std::string>(data, "type");
    result->allowsMultipleAnswers =
        parsePrimitive<bool>(data, "allows_multiple_answers");
    result->correctOptionId =
        parsePrimitive<std::int32_t>(data, "correct_option_id");
    result->explanation = parsePrimitive<std::string>(data, "explanation");
    result->explanationEntities =
        parseArray<MessageEntity>(data, "explanation_entities");
    result->openPeriod = parsePrimitive<std::int32_t>(data, "open_period");
    result->closeDate = parsePrimitive<std::int64_t>(data, "close_date");
    return result;
}

DECLARE_PARSER_TO_JSON(Poll) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("question", object->question);
        json.put("options", put(object->options));
        json.put("total_voter_count", object->totalVoterCount);
        json.put("is_closed", object->isClosed);
        json.put("is_anonymous", object->isAnonymous);
        json.put("type", object->type);
        json.put("allows_multiple_answers", object->allowsMultipleAnswers);
        json.put("correct_option_id", object->correctOptionId);
        json.put("explanation", object->explanation);
        json.put("explanation_entities", put(object->explanationEntities));
        json.put("open_period", object->openPeriod);
        json.put("close_date", object->closeDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Location) {
    auto result(std::make_shared<Location>());
    result->latitude = (float)parsePrimitive<double>(data, "latitude");
    result->longitude = (float)parsePrimitive<double>(data, "longitude");
    result->horizontalAccuracy =
        (float)parsePrimitive<double>(data, "horizontal_accuracy");
    result->livePeriod = parsePrimitive<std::int32_t>(data, "live_period");
    result->heading = parsePrimitive<std::int32_t>(data, "heading");
    result->proximityAlertRadius =
        parsePrimitive<std::int32_t>(data, "proximity_alert_radius");
    return result;
}

DECLARE_PARSER_TO_JSON(Location) {
    JsonWrapper json;
    if (object) {
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("horizontal_accuracy", object->horizontalAccuracy);
        json.put("live_period", object->livePeriod);
        json.put("heading", object->heading);
        json.put("proximity_alert_radius", object->proximityAlertRadius);
    }
    return json;
}
DECLARE_PARSER_FROM_JSON(Venue) {
    auto result = std::make_shared<Venue>();
    result->location = parse<Location>(data, "location");
    result->title = parsePrimitive<std::string>(data, "title");
    result->address = parsePrimitive<std::string>(data, "address");
    result->foursquareId = parsePrimitive<std::string>(data, "foursquare_id");
    result->foursquareType =
        parsePrimitive<std::string>(data, "foursquare_type");
    result->googlePlaceId =
        parsePrimitive<std::string>(data, "google_place_id");
    result->googlePlaceType =
        parsePrimitive<std::string>(data, "google_place_type");
    return result;
}

DECLARE_PARSER_TO_JSON(Venue) {
    JsonWrapper json;
    if (object) {
        json.put("location", put(object->location));
        json.put("title", object->title);
        json.put("address", object->address);
        json.put("foursquare_id", object->foursquareId);
        json.put("foursquare_type", object->foursquareType);
        json.put("google_place_id", object->googlePlaceId);
        json.put("google_place_type", object->googlePlaceType);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(WebAppData) {
    auto result = std::make_shared<WebAppData>();
    result->data = parsePrimitive<std::string>(data, "data");
    result->buttonText = parsePrimitive<std::string>(data, "button_text");
    return result;
}

DECLARE_PARSER_TO_JSON(WebAppData) {
    JsonWrapper json;
    if (object) {
        json.put("data", object->data);
        json.put("button_text", object->buttonText);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ProximityAlertTriggered) {
    auto result = std::make_shared<ProximityAlertTriggered>();
    result->traveler = parse<User>(data, "traveler");
    result->watcher = parse<User>(data, "watcher");
    result->distance = parsePrimitive<std::int32_t>(data, "distance");
    return result;
}

DECLARE_PARSER_TO_JSON(ProximityAlertTriggered) {
    JsonWrapper json;
    if (object) {
        json.put("traveler", put(object->traveler));
        json.put("watcher", put(object->watcher));
        json.put("distance", object->distance);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(MessageAutoDeleteTimerChanged) {
    auto result = std::make_shared<MessageAutoDeleteTimerChanged>();
    result->messageAutoDeleteTime =
        parsePrimitive<std::int32_t>(data, "message_auto_delete_time");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageAutoDeleteTimerChanged) {
    JsonWrapper json;
    if (object) {
        json.put("message_auto_delete_time", object->messageAutoDeleteTime);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatBoostAdded) {
    auto result = std::make_shared<ChatBoostAdded>();
    result->boostCount = parsePrimitive<std::int32_t>(data, "boost_count");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBoostAdded) {
    JsonWrapper json;
    if (object) {
        json.put("boost_count", object->boostCount);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ForumTopicCreated) {
    auto result = std::make_shared<ForumTopicCreated>();
    result->name = parsePrimitive<std::string>(data, "name");
    result->iconColor = parsePrimitive<std::int32_t>(data, "icon_color");
    result->iconCustomEmojiId =
        parsePrimitive<std::string>(data, "icon_custom_emoji_id");
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopicCreated) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("icon_color", object->iconColor);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ForumTopicClosed) {
    return std::make_shared<ForumTopicClosed>();
}

DECLARE_PARSER_TO_JSON(ForumTopicClosed) {
    return JsonWrapper();  // Empty JSON for closed topic
}

DECLARE_PARSER_FROM_JSON(ForumTopicEdited) {
    auto result = std::make_shared<ForumTopicEdited>();
    result->name = parsePrimitive<std::string>(data, "name");
    result->iconCustomEmojiId =
        parsePrimitive<std::string>(data, "icon_custom_emoji_id");
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopicEdited) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
    }
    return json;
}
DECLARE_PARSER_FROM_JSON(ForumTopicReopened) {
    return std::make_shared<ForumTopicReopened>();
}

DECLARE_PARSER_TO_JSON(ForumTopicReopened) {
    return JsonWrapper();  // Empty JSON for reopened topic
}

DECLARE_PARSER_FROM_JSON(GeneralForumTopicHidden) {
    return std::make_shared<GeneralForumTopicHidden>();
}

DECLARE_PARSER_TO_JSON(GeneralForumTopicHidden) {
    return JsonWrapper();  // Empty JSON for hidden topic
}

DECLARE_PARSER_FROM_JSON(GeneralForumTopicUnhidden) {
    return std::make_shared<GeneralForumTopicUnhidden>();
}

DECLARE_PARSER_TO_JSON(GeneralForumTopicUnhidden) {
    return JsonWrapper();  // Empty JSON for unhidden topic
}

DECLARE_PARSER_FROM_JSON(SharedUser) {
    auto result = std::make_shared<SharedUser>();
    result->userId = parsePrimitive<std::int64_t>(data, "user_id");
    result->firstName = parsePrimitive<std::string>(data, "first_name");
    result->lastName = parsePrimitive<std::string>(data, "last_name");
    result->username = parsePrimitive<std::string>(data, "username");
    result->photo = parseArray<PhotoSize>(data, "photo");
    return result;
}

DECLARE_PARSER_TO_JSON(SharedUser) {
    JsonWrapper json;
    if (object) {
        json.put("user_id", object->userId);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("username", object->username);
        json.put("photo", put(object->photo));
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UsersShared) {
    auto result = std::make_shared<UsersShared>();
    result->requestId = parsePrimitive<std::int32_t>(data, "request_id");
    result->users = parseArray<SharedUser>(data, "users");
    return result;
}

DECLARE_PARSER_TO_JSON(UsersShared) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("users", put(object->users));
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatShared) {
    auto result = std::make_shared<ChatShared>();
    result->requestId = parsePrimitive<std::int32_t>(data, "request_id");
    result->chatId = parsePrimitive<std::int64_t>(data, "chat_id");
    result->title = parsePrimitive<std::string>(data, "title");
    result->username = parsePrimitive<std::string>(data, "username");
    result->photo = parseArray<PhotoSize>(data, "photo");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatShared) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("chat_id", object->chatId);
        json.put("title", object->title);
        json.put("username", object->username);
        json.put("photo", put(object->photo));
    }
    return json;
}
DECLARE_PARSER_FROM_JSON(WriteAccessAllowed) {
    auto result = std::make_shared<WriteAccessAllowed>();
    result->fromRequest = parsePrimitive<bool>(data, "from_request");
    result->webAppName = parsePrimitive<std::string>(data, "web_app_name");
    result->fromAttachmentMenu =
        parsePrimitive<bool>(data, "from_attachment_menu");
    return result;
}

DECLARE_PARSER_TO_JSON(WriteAccessAllowed) {
    JsonWrapper json;
    if (object) {
        json.put("from_request", object->fromRequest);
        json.put("web_app_name", object->webAppName);
        json.put("from_attachment_menu", object->fromAttachmentMenu);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(VideoChatScheduled) {
    auto result = std::make_shared<VideoChatScheduled>();
    result->startDate = parsePrimitive<std::int32_t>(data, "start_date");
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatScheduled) {
    JsonWrapper json;
    if (object) {
        json.put("start_date", object->startDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(VideoChatStarted) {
    return std::make_shared<VideoChatStarted>();  // No fields to parse
}

DECLARE_PARSER_TO_JSON(VideoChatStarted) {
    return JsonWrapper();  // Empty JSON for started chat
}

DECLARE_PARSER_FROM_JSON(VideoChatEnded) {
    auto result = std::make_shared<VideoChatEnded>();
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatEnded) {
    JsonWrapper json;
    if (object) {
        json.put("duration", object->duration);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(VideoChatParticipantsInvited) {
    auto result = std::make_shared<VideoChatParticipantsInvited>();
    result->users = parseArray<User>(data, "users");
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatParticipantsInvited) {
    JsonWrapper json;
    if (object) {
        json.put("users", put(object->users));
    }
    return json;
}

// GiveawayCreated Parser
DECLARE_PARSER_FROM_JSON(GiveawayCreated) {
    return std::make_shared<GiveawayCreated>();  // No fields to parse
}

DECLARE_PARSER_TO_JSON(GiveawayCreated) {
    return JsonWrapper();  // Empty JSON
}

// Giveaway Parser
DECLARE_PARSER_FROM_JSON(Giveaway) {
    auto result = std::make_shared<Giveaway>();
    result->chats = parseArray<Chat>(data, "chats");
    result->winnersSelectionDate =
        parsePrimitive<std::uint32_t>(data, "winners_selection_date");
    result->winnerCount = parsePrimitive<std::int32_t>(data, "winner_count");
    result->onlyNewMembers = parsePrimitive<bool>(data, "only_new_members");
    result->hasPublicWinners = parsePrimitive<bool>(data, "has_public_winners");
    result->prizeDescription =
        parsePrimitive<std::string>(data, "prize_description");
    result->countryCodes =
        parsePrimitiveArray<std::string>(data, "country_codes");
    result->premiumSubscriptionMonthCount =
        parsePrimitive<std::int32_t>(data, "premium_subscription_month_count");
    return result;
}

DECLARE_PARSER_TO_JSON(Giveaway) {
    JsonWrapper json;
    if (object) {
        json.put("chats", put(object->chats));
        json.put("winners_selection_date", object->winnersSelectionDate);
        json.put("winner_count", object->winnerCount);
        json.put("only_new_members", object->onlyNewMembers);
        json.put("has_public_winners", object->hasPublicWinners);
        json.put("prize_description", object->prizeDescription);
        json.put("country_codes", put(object->countryCodes));
        json.put("premium_subscription_month_count",
                 object->premiumSubscriptionMonthCount);
    }
    return json;
}

// GiveawayWinners Parser
DECLARE_PARSER_FROM_JSON(GiveawayWinners) {
    auto result = std::make_shared<GiveawayWinners>();
    result->chat = parse<Chat>(data, "chat");
    result->giveawayMessageId =
        parsePrimitive<std::int32_t>(data, "giveaway_message_id");
    result->winnersSelectionDate =
        parsePrimitive<std::uint32_t>(data, "winners_selection_date");
    result->winnerCount = parsePrimitive<std::int32_t>(data, "winner_count");
    result->winners = parseArray<User>(data, "winners");
    result->additionalChatCount =
        parsePrimitive<std::int32_t>(data, "additional_chat_count");
    result->premiumSubscriptionMonthCount =
        parsePrimitive<std::int32_t>(data, "premium_subscription_month_count");
    result->unclaimedPrizeCount =
        parsePrimitive<std::int32_t>(data, "unclaimed_prize_count");
    result->onlyNewMembers = parsePrimitive<bool>(data, "only_new_members");
    result->wasRefunded = parsePrimitive<bool>(data, "was_refunded");
    result->prizeDescription =
        parsePrimitive<std::string>(data, "prize_description");
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayWinners) {
    JsonWrapper json;
    if (object) {
        json.put("giveaway_message_id", object->giveawayMessageId);
        json.put("winners_selection_date", object->winnersSelectionDate);
        json.put("winner_count", object->winnerCount);
        json.put("winners", put(object->winners));
        json.put("additional_chat_count", object->additionalChatCount);
        json.put("premium_subscription_month_count",
                 object->premiumSubscriptionMonthCount);
        json.put("unclaimed_prize_count", object->unclaimedPrizeCount);
        json.put("only_new_members", object->onlyNewMembers);
        json.put("was_refunded", object->wasRefunded);
        json.put("prize_description", object->prizeDescription);
    }
    return json;
}
// GiveawayCompleted Parser
DECLARE_PARSER_FROM_JSON(GiveawayCompleted) {
    auto result = std::make_shared<GiveawayCompleted>();
    result->winnerCount = parsePrimitive<std::int32_t>(data, "winner_count");
    result->unclaimedPrizeCount =
        parsePrimitive<std::int32_t>(data, "unclaimed_prize_count");
    result->giveawayMessage = parse<Message>(data, "giveaway_message");
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayCompleted) {
    JsonWrapper json;
    if (object) {
        json.put("winner_count", object->winnerCount);
        json.put("unclaimed_prize_count", object->unclaimedPrizeCount);
        json.put("giveaway_message", put(object->giveawayMessage));
    }
    return json;
}

// LinkPreviewOptions Parser
DECLARE_PARSER_FROM_JSON(LinkPreviewOptions) {
    auto result = std::make_shared<LinkPreviewOptions>();
    result->isDisabled = parsePrimitive<bool>(data, "is_disabled");
    result->url = parsePrimitive<std::string>(data, "url");
    result->preferSmallMedia = parsePrimitive<bool>(data, "prefer_small_media");
    result->preferLargeMedia = parsePrimitive<bool>(data, "prefer_large_media");
    result->showAboveText = parsePrimitive<bool>(data, "show_above_text");
    return result;
}

DECLARE_PARSER_TO_JSON(LinkPreviewOptions) {
    JsonWrapper json;
    if (object) {
        json.put("is_disabled", object->isDisabled);
        json.put("url", object->url);
        json.put("prefer_small_media", object->preferSmallMedia);
        json.put("prefer_large_media", object->preferLargeMedia);
        json.put("show_above_text", object->showAboveText);
    }
    return json;
}

// UserProfilePhotos Parser
DECLARE_PARSER_FROM_JSON(UserProfilePhotos) {
    auto result = std::make_shared<UserProfilePhotos>();
    result->totalCount = parsePrimitive<std::int32_t>(data, "total_count");
    result->photos = parseMatrix<PhotoSize>(data, "photos");
    return result;
}

DECLARE_PARSER_TO_JSON(UserProfilePhotos) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("photos", put(object->photos));
    }
    return json;
}

// File Parser
DECLARE_PARSER_FROM_JSON(File) {
    auto result = std::make_shared<File>();
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->fileSize = parsePrimitive<std::int64_t>(data, "file_size");
    result->filePath = parsePrimitive<std::string>(data, "file_path");
    return result;
}

DECLARE_PARSER_TO_JSON(File) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("file_size", object->fileSize);
        json.put("file_path", object->filePath);
    }
    return json;
}

// WebAppInfo Parser
DECLARE_PARSER_FROM_JSON(WebAppInfo) {
    auto result = std::make_shared<WebAppInfo>();
    result->url = parsePrimitive<std::string>(data, "url");
    return result;
}

DECLARE_PARSER_TO_JSON(WebAppInfo) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
    }
    return json;
}

// ReplyKeyboardMarkup Parser
DECLARE_PARSER_FROM_JSON(ReplyKeyboardMarkup) {
    auto result = std::make_shared<ReplyKeyboardMarkup>();
    result->keyboard = parseMatrix<KeyboardButton>(data, "keyboard");
    result->isPersistent = parsePrimitive<bool>(data, "is_persistent");
    result->resizeKeyboard = parsePrimitive<bool>(data, "resize_keyboard");
    result->oneTimeKeyboard = parsePrimitive<bool>(data, "one_time_keyboard");
    result->inputFieldPlaceholder =
        parsePrimitive<std::string>(data, "input_field_placeholder");
    result->selective = parsePrimitive<bool>(data, "selective");
    return result;
}

DECLARE_PARSER_TO_JSON(ReplyKeyboardMarkup) {
    JsonWrapper json;
    if (object) {
        json.put("keyboard", put(object->keyboard));
        json.put("is_persistent", object->isPersistent);
        json.put("resize_keyboard", object->resizeKeyboard);
        json.put("one_time_keyboard", object->oneTimeKeyboard);
        json.put("input_field_placeholder", object->inputFieldPlaceholder);
        json.put("selective", object->selective);
    }
    return json;
}

// KeyboardButton Parser
DECLARE_PARSER_FROM_JSON(KeyboardButton) {
    auto result = std::make_shared<KeyboardButton>();
    result->text = parsePrimitive<std::string>(data, "text");
    result->requestUsers =
        parse<KeyboardButtonRequestUsers>(data, "request_users");
    result->requestChat =
        parse<KeyboardButtonRequestChat>(data, "request_chat");
    result->requestContact = parsePrimitive<bool>(data, "request_contact");
    result->requestLocation = parsePrimitive<bool>(data, "request_location");
    result->requestPoll = parse<KeyboardButtonPollType>(data, "request_poll");
    result->webApp = parse<WebAppInfo>(data, "web_app");
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButton) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("request_users", put(object->requestUsers));
        json.put("request_chat", put(object->requestChat));
        json.put("request_contact", object->requestContact);
        json.put("request_location", object->requestLocation);
        json.put("request_poll", put(object->requestPoll));
        json.put("web_app", put(object->webApp));
    }
    return json;
}

// KeyboardButtonRequestUsers Parser
DECLARE_PARSER_FROM_JSON(KeyboardButtonRequestUsers) {
    auto result = std::make_shared<KeyboardButtonRequestUsers>();
    result->requestId = parsePrimitive<std::int32_t>(data, "request_id");
    result->userIsBot = parsePrimitive<bool>(data, "user_is_bot");
    result->userIsPremium = parsePrimitive<bool>(data, "user_is_premium");
    result->maxQuantity =
        (std::uint8_t)parsePrimitive<std::int32_t>(data, "max_quantity");
    result->requestName = parsePrimitive<bool>(data, "request_name");
    result->requestUsername = parsePrimitive<bool>(data, "request_username");
    result->requestPhoto = parsePrimitive<bool>(data, "request_photo");
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonRequestUsers) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("user_is_bot", object->userIsBot);
        json.put("user_is_premium", object->userIsPremium);
        json.put("max_quantity", object->maxQuantity);
        json.put("request_name", object->requestName);
        json.put("request_username", object->requestUsername);
        json.put("request_photo", object->requestPhoto);
    }
    return json;
}

// Function for parsing KeyboardButtonRequestChat from JSON
DECLARE_PARSER_FROM_JSON(KeyboardButtonRequestChat) {
    auto result = std::make_shared<KeyboardButtonRequestChat>();
    result->requestId = parsePrimitive<std::int32_t>(data, "request_id");
    result->chatIsChannel = parsePrimitive<bool>(data, "chat_is_channel");
    result->chatIsForum = parsePrimitive<bool>(data, "chat_is_forum");
    result->chatHasUsername = parsePrimitive<bool>(data, "chat_has_username");
    result->chatIsCreated = parsePrimitive<bool>(data, "chat_is_created");
    result->userAdministratorRights =
        parse<ChatAdministratorRights>(data, "user_administrator_rights");
    result->botAdministratorRights =
        parse<ChatAdministratorRights>(data, "bot_administrator_rights");
    result->botIsMember = parsePrimitive<bool>(data, "bot_is_member");
    result->requestTitle = parsePrimitive<bool>(data, "request_title");
    result->requestUsername = parsePrimitive<bool>(data, "request_username");
    result->requestPhoto = parsePrimitive<bool>(data, "request_photo");
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonRequestChat) {
    if (!object) return {};
    JsonWrapper json;
    json.put("request_id", object->requestId);
    json.put("chat_is_channel", object->chatIsChannel);
    json.put("chat_is_forum", object->chatIsForum);
    json.put("chat_has_username", object->chatHasUsername);
    json.put("chat_is_created", object->chatIsCreated);
    json.put("user_administrator_rights", put(object->userAdministratorRights));
    json.put("bot_administrator_rights", put(object->botAdministratorRights));
    json.put("bot_is_member", object->botIsMember);
    json.put("request_title", object->requestTitle);
    json.put("request_username", object->requestUsername);
    json.put("request_photo", object->requestPhoto);
    return json;
}

// Function for parsing KeyboardButtonPollType from JSON
DECLARE_PARSER_FROM_JSON(KeyboardButtonPollType) {
    auto result = std::make_shared<KeyboardButtonPollType>();
    result->type = parsePrimitive<std::string>(data, "type");
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonPollType) {
    if (!object) return {};
    JsonWrapper json;
    json.put("type", object->type);
    return json;
}

// Function for parsing ReplyKeyboardRemove from JSON
DECLARE_PARSER_FROM_JSON(ReplyKeyboardRemove) {
    auto result = std::make_shared<ReplyKeyboardRemove>();
    result->removeKeyboard = parsePrimitive<bool>(data, "remove_keyboard");
    result->selective = parsePrimitive<bool>(data, "selective");
    return result;
}

DECLARE_PARSER_TO_JSON(ReplyKeyboardRemove) {
    if (!object) return {};
    JsonWrapper json;
    json.put("remove_keyboard", object->removeKeyboard);
    json.put("selective", object->selective);
    return json;
}

// Function for parsing InlineKeyboardMarkup from JSON
DECLARE_PARSER_FROM_JSON(InlineKeyboardMarkup) {
    auto result = std::make_shared<InlineKeyboardMarkup>();
    result->inlineKeyboard =
        parseMatrix<InlineKeyboardButton>(data, "inline_keyboard");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineKeyboardMarkup) {
    if (!object) return {};
    JsonWrapper json;
    json.put("inline_keyboard", put(object->inlineKeyboard));
    return json;
}
DECLARE_PARSER_FROM_JSON(ForceReply) {
    auto result = std::make_shared<ForceReply>();
    result->forceReply = parsePrimitive<bool>(data, "force_reply");
    result->inputFieldPlaceholder =
        parsePrimitive<std::string>(data, "input_field_placeholder");
    result->selective = parsePrimitive<bool>(data, "selective");
    return result;
}

DECLARE_PARSER_TO_JSON(ForceReply) {
    if (!object) return {};
    JsonWrapper json;
    json.put("force_reply", object->forceReply);
    json.put("input_field_placeholder", object->inputFieldPlaceholder);
    json.put("selective", object->selective);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatPhoto) {
    auto result = std::make_shared<ChatPhoto>();
    result->smallFileId = parsePrimitive<std::string>(data, "small_file_id");
    result->smallFileUniqueId =
        parsePrimitive<std::string>(data, "small_file_unique_id");
    result->bigFileId = parsePrimitive<std::string>(data, "big_file_id");
    result->bigFileUniqueId =
        parsePrimitive<std::string>(data, "big_file_unique_id");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatPhoto) {
    if (!object) return {};
    JsonWrapper json;
    json.put("small_file_id", object->smallFileId);
    json.put("small_file_unique_id", object->smallFileUniqueId);
    json.put("big_file_id", object->bigFileId);
    json.put("big_file_unique_id", object->bigFileUniqueId);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatInviteLink) {
    auto result = std::make_shared<ChatInviteLink>();
    result->inviteLink = parsePrimitive<std::string>(data, "invite_link");
    result->creator = parse<User>(data, "creator");
    result->createsJoinRequest =
        parsePrimitive<bool>(data, "creates_join_request");
    result->isPrimary = parsePrimitive<bool>(data, "is_primary");
    result->isRevoked = parsePrimitive<bool>(data, "is_revoked");
    result->name = parsePrimitive<std::string>(data, "name");
    result->expireDate = parsePrimitive<std::uint32_t>(data, "expire_date");
    result->memberLimit = parsePrimitive<std::uint32_t>(data, "member_limit");
    result->pendingJoinRequestCount =
        parsePrimitive<std::uint32_t>(data, "pending_join_request_count");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatInviteLink) {
    if (!object) return {};
    JsonWrapper json;
    json.put("invite_link", object->inviteLink);
    json.put("creator", put(object->creator));
    json.put("creates_join_request", object->createsJoinRequest);
    json.put("is_primary", object->isPrimary);
    json.put("is_revoked", object->isRevoked);
    json.put("name", object->name);
    json.put("expire_date", object->expireDate);
    json.put("member_limit", object->memberLimit);
    json.put("pending_join_request_count", object->pendingJoinRequestCount);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatAdministratorRights) {
    auto result = std::make_shared<ChatAdministratorRights>();
    result->isAnonymous = parsePrimitive<bool>(data, "is_anonymous");
    result->canManageChat = parsePrimitive<bool>(data, "can_manage_chat");
    result->canDeleteMessages =
        parsePrimitive<bool>(data, "can_delete_messages");
    result->canManageVideoChats =
        parsePrimitive<bool>(data, "can_manage_video_chats");
    result->canRestrictMembers =
        parsePrimitive<bool>(data, "can_restrict_members");
    result->canPromoteMembers =
        parsePrimitive<bool>(data, "can_promote_members");
    result->canChangeInfo = parsePrimitive<bool>(data, "can_change_info");
    result->canInviteUsers = parsePrimitive<bool>(data, "can_invite_users");
    result->canPostStories = parsePrimitive<bool>(data, "can_post_stories");
    result->canEditStories = parsePrimitive<bool>(data, "can_edit_stories");
    result->canDeleteStories = parsePrimitive<bool>(data, "can_delete_stories");
    result->canPostMessages = parsePrimitive<bool>(data, "can_post_messages");
    result->canEditMessages = parsePrimitive<bool>(data, "can_edit_messages");
    result->canPinMessages = parsePrimitive<bool>(data, "can_pin_messages");
    result->canManageTopics = parsePrimitive<bool>(data, "can_manage_topics");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatAdministratorRights) {
    if (!object) return {};
    JsonWrapper json;
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
    return json;
}
DECLARE_PARSER_FROM_JSON(ChatMemberUpdated) {
    auto result = std::make_shared<ChatMemberUpdated>();
    result->chat = parse<Chat>(data, "chat");
    result->from = parse<User>(data, "from");
    result->date = parsePrimitive<std::uint32_t>(data, "date");
    result->oldChatMember = parse<ChatMember>(data, "old_chat_member");
    result->newChatMember = parse<ChatMember>(data, "new_chat_member");
    result->inviteLink = parse<ChatInviteLink>(data, "invite_link");
    result->viaChatFolderInviteLink =
        parsePrimitive<bool>(data, "via_chat_folder_invite_link");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberUpdated) {
    if (!object) return {};
    JsonWrapper json;
    json.put("chat", put(object->chat));
    json.put("from", put(object->from));
    json.put("date", object->date);
    json.put("old_chat_member", put(object->oldChatMember));
    json.put("new_chat_member", put(object->newChatMember));
    json.put("invite_link", put(object->inviteLink));
    json.put("via_chat_folder_invite_link", object->viaChatFolderInviteLink);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatMember) {
    auto result = std::make_shared<ChatMember>();
    std::string status = parsePrimitive<std::string>(data, "status");
    if (status == ChatMemberOwner::STATUS) {
        result = parse<ChatMemberOwner>(data);
    } else if (status == ChatMemberAdministrator::STATUS) {
        result = parse<ChatMemberAdministrator>(data);
    } else if (status == ChatMemberMember::STATUS) {
        result = parse<ChatMemberMember>(data);
    } else if (status == ChatMemberRestricted::STATUS) {
        result = parse<ChatMemberRestricted>(data);
    } else if (status == ChatMemberLeft::STATUS) {
        result = parse<ChatMemberLeft>(data);
    } else if (status == ChatMemberBanned::STATUS) {
        result = parse<ChatMemberBanned>(data);
    }
    result->status = status;
    result->user = parse<User>(data, "user");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMember) {
    if (!object) return {};
    JsonWrapper json;
    json.put("status", object->status);
    json.put("user", put(object->user));

    if (object->status == ChatMemberOwner::STATUS) {
        json.put("extra", put<ChatMemberOwner>(object));
    } else if (object->status == ChatMemberAdministrator::STATUS) {
        json.put("extra", put<ChatMemberAdministrator>(object));
    } else if (object->status == ChatMemberMember::STATUS) {
        json.put("extra", put<ChatMemberMember>(object));
    } else if (object->status == ChatMemberRestricted::STATUS) {
        json.put("extra", put<ChatMemberRestricted>(object));
    } else if (object->status == ChatMemberLeft::STATUS) {
        json.put("extra", put<ChatMemberLeft>(object));
    } else if (object->status == ChatMemberBanned::STATUS) {
        json.put("extra", put<ChatMemberBanned>(object));
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatMemberOwner) {
    auto result = std::make_shared<ChatMemberOwner>();
    result->isAnonymous = parsePrimitive<bool>(data, "is_anonymous");
    result->customTitle = parsePrimitive<std::string>(data, "custom_title");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberOwner) {
    if (!object) return {};
    JsonWrapper json;
    json.put("is_anonymous", object->isAnonymous);
    json.put("custom_title", object->customTitle);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatMemberAdministrator) {
    auto result = std::make_shared<ChatMemberAdministrator>();
    result->canBeEdited = parsePrimitive<bool>(data, "can_be_edited");
    result->isAnonymous = parsePrimitive<bool>(data, "is_anonymous");
    result->canManageChat = parsePrimitive<bool>(data, "can_manage_chat");
    result->canDeleteMessages =
        parsePrimitive<bool>(data, "can_delete_messages");
    result->canManageVideoChats =
        parsePrimitive<bool>(data, "can_manage_video_chats");
    result->canRestrictMembers =
        parsePrimitive<bool>(data, "can_restrict_members");
    result->canPromoteMembers =
        parsePrimitive<bool>(data, "can_promote_members");
    result->canChangeInfo = parsePrimitive<bool>(data, "can_change_info");
    result->canInviteUsers = parsePrimitive<bool>(data, "can_invite_users");
    result->canPostStories = parsePrimitive<bool>(data, "can_post_stories");
    result->canEditStories = parsePrimitive<bool>(data, "can_edit_stories");
    result->canDeleteStories = parsePrimitive<bool>(data, "can_delete_stories");
    result->canPostMessages = parsePrimitive<bool>(data, "can_post_messages");
    result->canEditMessages = parsePrimitive<bool>(data, "can_edit_messages");
    result->canPinMessages = parsePrimitive<bool>(data, "can_pin_messages");
    result->canManageTopics = parsePrimitive<bool>(data, "can_manage_topics");
    result->customTitle = parsePrimitive<std::string>(data, "custom_title");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberAdministrator) {
    if (!object) return {};
    JsonWrapper json;
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
    json.put("custom_title", object->customTitle);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatMemberMember) {
    return std::make_shared<ChatMemberMember>();
}

DECLARE_PARSER_TO_JSON(ChatMemberMember) {
    if (!object) return {};
    return {};
}

DECLARE_PARSER_FROM_JSON(ChatMemberRestricted) {
    auto result = std::make_shared<ChatMemberRestricted>();
    result->isMember = parsePrimitive<bool>(data, "is_member");
    result->canSendMessages = parsePrimitive<bool>(data, "can_send_messages");
    result->canSendAudios = parsePrimitive<bool>(data, "can_send_audios");
    result->canSendDocuments = parsePrimitive<bool>(data, "can_send_documents");
    result->canSendPhotos = parsePrimitive<bool>(data, "can_send_photos");
    result->canSendVideos = parsePrimitive<bool>(data, "can_send_videos");
    result->canSendVideoNotes =
        parsePrimitive<bool>(data, "can_send_video_notes");
    result->canSendVoiceNotes =
        parsePrimitive<bool>(data, "can_send_voice_notes");
    result->canSendPolls = parsePrimitive<bool>(data, "can_send_polls");
    result->canSendOtherMessages =
        parsePrimitive<bool>(data, "can_send_other_messages");
    result->canAddWebPagePreviews =
        parsePrimitive<bool>(data, "can_add_web_page_previews");
    result->canChangeInfo = parsePrimitive<bool>(data, "can_change_info");
    result->canInviteUsers = parsePrimitive<bool>(data, "can_invite_users");
    result->canPinMessages = parsePrimitive<bool>(data, "can_pin_messages");
    result->canManageTopics = parsePrimitive<bool>(data, "can_manage_topics");
    result->untilDate = parsePrimitive<std::uint32_t>(data, "until_date");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberRestricted) {
    if (!object) return {};
    JsonWrapper json;
    json.put("is_member", object->isMember);
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
    json.put("can_change_info", object->canChangeInfo);
    json.put("can_invite_users", object->canInviteUsers);
    json.put("can_pin_messages", object->canPinMessages);
    json.put("can_manage_topics", object->canManageTopics);
    json.put("until_date", object->untilDate);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatMemberLeft) {
    return std::make_shared<ChatMemberLeft>();
}

DECLARE_PARSER_TO_JSON(ChatMemberLeft) {
    if (!object) return {};
    return {};
}

DECLARE_PARSER_FROM_JSON(ChatMemberBanned) {
    auto result = std::make_shared<ChatMemberBanned>();
    result->untilDate = parsePrimitive<std::uint32_t>(data, "until_date");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberBanned) {
    if (!object) return {};
    JsonWrapper json;
    json.put("until_date", object->untilDate);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatJoinRequest) {
    auto result = std::make_shared<ChatJoinRequest>();
    result->chat = parse<Chat>(data, "chat");
    result->from = parse<User>(data, "from");
    result->userChatId = parsePrimitive<std::int64_t>(data, "user_chat_id");
    result->date = parsePrimitive<std::int32_t>(data, "date");
    result->bio = parsePrimitive<std::string>(data, "bio");
    result->inviteLink = parse<ChatInviteLink>(data, "invite_link");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatJoinRequest) {
    if (!object) return {};
    JsonWrapper json;
    json.put("chat", put(object->chat));
    json.put("from", put(object->from));
    json.put("user_chat_id", object->userChatId);
    json.put("date", object->date);
    json.put("bio", object->bio);
    json.put("invite_link", put(object->inviteLink));
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatPermissions) {
    auto result = std::make_shared<ChatPermissions>();
    result->canSendMessages = parsePrimitive<bool>(data, "can_send_messages");
    result->canSendAudios = parsePrimitive<bool>(data, "can_send_audios");
    result->canSendDocuments = parsePrimitive<bool>(data, "can_send_documents");
    result->canSendPhotos = parsePrimitive<bool>(data, "can_send_photos");
    result->canSendVideos = parsePrimitive<bool>(data, "can_send_videos");
    result->canSendVideoNotes =
        parsePrimitive<bool>(data, "can_send_video_notes");
    result->canSendVoiceNotes =
        parsePrimitive<bool>(data, "can_send_voice_notes");
    result->canSendPolls = parsePrimitive<bool>(data, "can_send_polls");
    result->canSendOtherMessages =
        parsePrimitive<bool>(data, "can_send_other_messages");
    result->canAddWebPagePreviews =
        parsePrimitive<bool>(data, "can_add_web_page_previews");
    result->canChangeInfo = parsePrimitive<bool>(data, "can_change_info");
    result->canInviteUsers = parsePrimitive<bool>(data, "can_invite_users");
    result->canPinMessages = parsePrimitive<bool>(data, "can_pin_messages");
    result->canManageTopics = parsePrimitive<bool>(data, "can_manage_topics");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatPermissions) {
    if (!object) return {};
    JsonWrapper json;
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
    json.put("can_change_info", object->canChangeInfo);
    json.put("can_invite_users", object->canInviteUsers);
    json.put("can_pin_messages", object->canPinMessages);
    json.put("can_manage_topics", object->canManageTopics);
    return json;
}
DECLARE_PARSER_FROM_JSON(Birthdate) {
    auto result = std::make_shared<Birthdate>();
    result->day = (std::uint8_t)parsePrimitive<std::int32_t>(data, "day");
    result->month = (std::uint8_t)parsePrimitive<std::int32_t>(data, "month");
    result->year = (std::uint16_t)parsePrimitive<std::int32_t>(data, "year");
    return result;
}

DECLARE_PARSER_TO_JSON(Birthdate) {
    if (!object) return {};
    JsonWrapper json;
    json.put("day", object->day);
    json.put("month", object->month);
    json.put("year", object->year);
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessIntro) {
    auto result = std::make_shared<BusinessIntro>();
    result->title = parsePrimitive<std::string>(data, "title");
    result->message = parsePrimitive<std::string>(data, "message");
    result->sticker = parse<Sticker>(data, "sticker");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessIntro) {
    if (!object) return {};
    JsonWrapper json;
    json.put("title", object->title);
    json.put("message", object->message);
    json.put("sticker", put(object->sticker));
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessLocation) {
    auto result = std::make_shared<BusinessLocation>();
    result->address = parsePrimitive<std::string>(data, "address");
    result->location = parse<Location>(data, "location");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessLocation) {
    if (!object) return {};
    JsonWrapper json;
    json.put("address", object->address);
    json.put("location", put(object->location));
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessOpeningHoursInterval) {
    auto result = std::make_shared<BusinessOpeningHoursInterval>();
    result->openingMinute =
        parsePrimitive<std::int32_t>(data, "opening_minute");
    result->closingMinute =
        parsePrimitive<std::int32_t>(data, "closing_minute");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessOpeningHoursInterval) {
    if (!object) return {};
    JsonWrapper json;
    json.put("opening_minute", object->openingMinute);
    json.put("closing_minute", object->closingMinute);
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessOpeningHours) {
    auto result = std::make_shared<BusinessOpeningHours>();
    result->timeZoneName = parsePrimitive<std::string>(data, "time_zone_name");
    result->openingHours =
        parseArray<BusinessOpeningHoursInterval>(data, "opening_hours");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessOpeningHours) {
    if (!object) return {};
    JsonWrapper json;
    json.put("time_zone_name", object->timeZoneName);
    json.put("opening_hours", put(object->openingHours));
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatLocation) {
    auto result = std::make_shared<ChatLocation>();
    result->location = parse<Location>(data, "location");
    result->address = parsePrimitive<std::string>(data, "address");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatLocation) {
    if (!object) return {};
    JsonWrapper json;
    json.put("location", put(object->location));
    json.put("address", object->address);
    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionType) {
    auto type = parsePrimitive<std::string>(data, "type");
    ReactionType::Ptr result;

    if (type == ReactionTypeEmoji::TYPE) {
        result = parse<ReactionTypeEmoji>(data);
    } else if (type == ReactionTypeCustomEmoji::TYPE) {
        result = parse<ReactionTypeCustomEmoji>(data);
    } else {
        throw invalidType("reaction", type);
    }

    result->type = type;
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionType) {
    if (!object) return {};
    JsonWrapper json;
    json.put("type", object->type);

    if (object->type == ReactionTypeEmoji::TYPE) {
        json += put<ReactionTypeEmoji>(object);
    } else if (object->type == ReactionTypeCustomEmoji::TYPE) {
        json += put<ReactionTypeCustomEmoji>(object);
    } else {
        throw invalidType("reaction", object->type);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionTypeEmoji) {
    auto result = std::make_shared<ReactionTypeEmoji>();
    result->emoji = parsePrimitive<std::string>(data, "emoji");
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionTypeEmoji) {
    if (!object) return {};
    JsonWrapper json;
    json.put("emoji", object->emoji);
    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionTypeCustomEmoji) {
    auto result = std::make_shared<ReactionTypeCustomEmoji>();
    result->customEmojiId =
        parsePrimitive<std::string>(data, "custom_emoji_id");
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionTypeCustomEmoji) {
    if (!object) return {};
    JsonWrapper json;
    json.put("custom_emoji_id", object->customEmojiId);
    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionCount) {
    auto result = std::make_shared<ReactionCount>();
    result->type = parse<ReactionType>(data, "type");
    result->totalCount = parsePrimitive<std::int32_t>(data, "total_count");
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionCount) {
    if (!object) return {};
    JsonWrapper json;
    json.put("type", put(object->type));
    json.put("total_count", object->totalCount);
    return json;
}

DECLARE_PARSER_FROM_JSON(MessageReactionUpdated) {
    auto result = std::make_shared<MessageReactionUpdated>();
    result->chat = parse<Chat>(data, "chat");
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    result->user = parse<User>(data, "user");
    result->actorChat = parse<Chat>(data, "actor_chat");
    result->date = parsePrimitive<std::uint32_t>(data, "date");
    result->oldReaction = parseArray<ReactionType>(data, "old_reaction");
    result->newReaction = parseArray<ReactionType>(data, "new_reaction");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionUpdated) {
    if (!object) return {};
    JsonWrapper json;
    json.put("chat", put(object->chat));
    json.put("message_id", object->messageId);
    json.put("user", put(object->user));
    json.put("actor_chat", put(object->actorChat));
    json.put("date", object->date);
    json.put("old_reaction", put(object->oldReaction));
    json.put("new_reaction", put(object->newReaction));
    return json;
}

DECLARE_PARSER_FROM_JSON(MessageReactionCountUpdated) {
    auto result = std::make_shared<MessageReactionCountUpdated>();
    result->chat = parse<Chat>(data, "chat");
    result->messageId = parsePrimitive<std::int32_t>(data, "message_id");
    result->date = parsePrimitive<std::uint32_t>(data, "date");
    result->reactions = parseArray<ReactionCount>(data, "reactions");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionCountUpdated) {
    if (!object) return {};
    JsonWrapper json;
    json.put("chat", put(object->chat));
    json.put("message_id", object->messageId);
    json.put("date", object->date);
    json.put("reactions", put(object->reactions));
    return json;
}

DECLARE_PARSER_FROM_JSON(ForumTopic) {
    auto result = std::make_shared<ForumTopic>();
    result->messageThreadId =
        parsePrimitive<std::int32_t>(data, "message_thread_id");
    result->name = parsePrimitive<std::string>(data, "name");
    result->iconColor = parsePrimitive<std::int32_t>(data, "icon_color");
    result->iconCustomEmojiId =
        parsePrimitive<std::string>(data, "icon_custom_emoji_id");
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopic) {
    JsonWrapper ptree;
    if (object) {
        ptree.put("message_thread_id", object->messageThreadId);
        ptree.put("name", object->name);
        ptree.put("icon_color", object->iconColor);
        ptree.put("icon_custom_emoji_id", object->iconCustomEmojiId);
    }
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotCommand) {
    auto result = std::make_shared<BotCommand>();
    result->command = parsePrimitive<std::string>(data, "command");
    result->description = parsePrimitive<std::string>(data, "description");
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommand) {
    JsonWrapper ptree;
    if (object) {
        ptree.put("command", object->command);
        ptree.put("description", object->description);
    }
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotCommandScope) {
    auto type = parsePrimitive<std::string>(data, "type");
    BotCommandScope::Ptr result;

    if (type == BotCommandScopeDefault::TYPE) {
        result = parse<BotCommandScopeDefault>(data);
    } else if (type == BotCommandScopeAllPrivateChats::TYPE) {
        result = parse<BotCommandScopeAllPrivateChats>(data);
    } else if (type == BotCommandScopeAllGroupChats::TYPE) {
        result = parse<BotCommandScopeAllGroupChats>(data);
    } else if (type == BotCommandScopeAllChatAdministrators::TYPE) {
        result = parse<BotCommandScopeAllChatAdministrators>(data);
    } else if (type == BotCommandScopeChat::TYPE) {
        result = parse<BotCommandScopeChat>(data);
    } else if (type == BotCommandScopeChatAdministrators::TYPE) {
        result = parse<BotCommandScopeChatAdministrators>(data);
    } else if (type == BotCommandScopeChatMember::TYPE) {
        result = parse<BotCommandScopeChatMember>(data);
    } else {
        throw invalidType("BotCommandScope", type);
    }

    result->type = type;
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommandScope) {
    JsonWrapper ptree;
    if (!object) return {};  // Return an empty ptree if object is null

    ptree.put("type", object->type);
    if (object->type == BotCommandScopeDefault::TYPE) {
        ptree += put<BotCommandScopeDefault>(object);
    } else if (object->type == BotCommandScopeAllPrivateChats::TYPE) {
        ptree += put<BotCommandScopeAllPrivateChats>(object);
    } else if (object->type == BotCommandScopeAllGroupChats::TYPE) {
        ptree += put<BotCommandScopeAllGroupChats>(object);
    } else if (object->type == BotCommandScopeAllChatAdministrators::TYPE) {
        ptree += put<BotCommandScopeAllChatAdministrators>(object);
    } else if (object->type == BotCommandScopeChat::TYPE) {
        ptree += put<BotCommandScopeChat>(object);
    } else if (object->type == BotCommandScopeChatAdministrators::TYPE) {
        ptree += put<BotCommandScopeChatAdministrators>(object);
    } else if (object->type == BotCommandScopeChatMember::TYPE) {
        ptree += put<BotCommandScopeChatMember>(object);
    } else {
        throw invalidType("BotCommandScope", object->type);
    }
    return ptree;
}

// Parsing from JSON to BotCommandScopeChatAdministrators
DECLARE_PARSER_FROM_JSON(BotCommandScopeChatAdministrators) {
    auto result = std::make_shared<BotCommandScopeChatAdministrators>();
    result->chatId = parsePrimitive<std::int64_t>(data, "chat_id");
    return result;
}

// Serializing BotCommandScopeChatAdministrators to JSON
DECLARE_PARSER_TO_JSON(BotCommandScopeChatAdministrators) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("chat_id", object->chatId);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotCommandScopeChat) {
    auto result = std::make_shared<BotCommandScopeChat>();
    result->chatId = parsePrimitive<std::int64_t>(data, "chat_id");
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommandScopeChat) {
    JsonWrapper ptree;
    if (object) {
        ptree.put("chat_id", object->chatId);
    }
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotCommandScopeChatMember) {
    auto result = std::make_shared<BotCommandScopeChatMember>();
    result->chatId = parsePrimitive<std::int64_t>(data, "chat_id");
    result->userId = parsePrimitive<std::int64_t>(data, "user_id");
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommandScopeChatMember) {
    JsonWrapper ptree;
    if (object) {
        ptree.put("chat_id", object->chatId);
        ptree.put("user_id", object->userId);
    }
    return ptree;
}
// BotName Parsing
DECLARE_PARSER_TO_JSON(BotName) {
    JsonWrapper ptree;
    if (!object) {
        ptree.put("name", object->name);
    }
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotName) {
    auto result = std::make_shared<BotName>();
    result->name = parsePrimitive<std::string>(data, "name");
    return result;
}

// BotDescription Parsing
DECLARE_PARSER_TO_JSON(BotDescription) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("description", object->description);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotDescription) {
    auto result = std::make_shared<BotDescription>();
    result->description = parsePrimitive<std::string>(data, "description");
    return result;
}

// BotShortDescription Parsing
DECLARE_PARSER_TO_JSON(BotShortDescription) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("short_description", object->shortDescription);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotShortDescription) {
    auto result = std::make_shared<BotShortDescription>();
    result->shortDescription =
        parsePrimitive<std::string>(data, "short_description");
    return result;
}

// ChatBoostSource Parsing
DECLARE_PARSER_TO_JSON(ChatBoostSource) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("source", object->source);
    ptree.put("user", put(object->user));

    if (object->source == ChatBoostSourcePremium::SOURCE) {
        ptree += put<ChatBoostSourcePremium>(object);
    } else if (object->source == ChatBoostSourceGiftCode::SOURCE) {
        ptree += put<ChatBoostSourceGiftCode>(object);
    } else if (object->source == ChatBoostSourceGiveaway::SOURCE) {
        ptree += put<ChatBoostSourceGiveaway>(object);
    }
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoostSource) {
    std::string source = parsePrimitive<std::string>(data, "source");
    ChatBoostSource::Ptr result;

    if (source == ChatBoostSourcePremium::SOURCE) {
        result = parse<ChatBoostSourcePremium>(data);
    } else if (source == ChatBoostSourceGiftCode::SOURCE) {
        result = parse<ChatBoostSourceGiftCode>(data);
    } else if (source == ChatBoostSourceGiveaway::SOURCE) {
        result = parse<ChatBoostSourceGiveaway>(data);
    } else {
        result = std::make_shared<ChatBoostSource>();
    }

    result->source = source;
    result->user = parse<User>(data, "user");
    return result;
}

// MenuButton Parsing
DECLARE_PARSER_TO_JSON(MenuButton) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("type", object->type);

    if (object->type == MenuButtonCommands::TYPE) {
        ptree += put<MenuButtonCommands>(object);
    } else if (object->type == MenuButtonWebApp::TYPE) {
        ptree += put<MenuButtonWebApp>(object);
    } else if (object->type == MenuButtonDefault::TYPE) {
        ptree += put<MenuButtonDefault>(object);
    } else {
        throw invalidType("MenuButton", object->type);
    }
    return ptree;
}

DECLARE_PARSER_FROM_JSON(MenuButton) {
    auto type = parsePrimitive<std::string>(data, "type");
    MenuButton::Ptr result;

    if (type == MenuButtonCommands::TYPE) {
        result = parse<MenuButtonCommands>(data);
    } else if (type == MenuButtonWebApp::TYPE) {
        result = parse<MenuButtonWebApp>(data);
    } else if (type == MenuButtonDefault::TYPE) {
        result = parse<MenuButtonDefault>(data);
    } else {
        throw invalidType("MenuButton", type);
    }

    result->type = type;
    return result;
}

// ChatBoost Parsing
DECLARE_PARSER_TO_JSON(ChatBoost) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("boost_id", object->boostId);
    ptree.put("add_date", object->addDate);
    ptree.put("expiration_date", object->expirationDate);
    ptree.put("source", put(object->source));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoost) {
    auto result = std::make_shared<ChatBoost>();
    result->boostId = parsePrimitive<std::string>(data, "boost_id");
    result->addDate = parsePrimitive<std::uint32_t>(data, "add_date");
    result->expirationDate =
        parsePrimitive<std::uint32_t>(data, "expiration_date");
    result->source = parse<ChatBoostSource>(data, "source");
    return result;
}

// ChatBoostUpdated Parsing
DECLARE_PARSER_TO_JSON(ChatBoostUpdated) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("chat", put(object->chat));
    ptree.put("boost", put(object->boost));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoostUpdated) {
    auto result = std::make_shared<ChatBoostUpdated>();
    result->chat = parse<Chat>(data, "chat");
    result->boost = parse<ChatBoost>(data, "boost");
    return result;
}

// ChatBoostRemoved Parsing
DECLARE_PARSER_TO_JSON(ChatBoostRemoved) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("chat", put(object->chat));
    ptree.put("boost_id", object->boostId);
    ptree.put("remove_date", object->removeDate);
    ptree.put("source", put(object->source));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoostRemoved) {
    auto result = std::make_shared<ChatBoostRemoved>();
    result->chat = parse<Chat>(data, "chat");
    result->boostId = parsePrimitive<std::string>(data, "boost_id");
    result->removeDate = parsePrimitive<std::uint32_t>(data, "remove_date");
    result->source = parse<ChatBoostSource>(data, "source");
    return result;
}

// UserChatBoosts Parsing
DECLARE_PARSER_TO_JSON(UserChatBoosts) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("boosts", put(object->boosts));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(UserChatBoosts) {
    auto result = std::make_shared<UserChatBoosts>();
    result->boosts = parseArray<ChatBoost>(data, "boosts");
    return result;
}

// MenuButton Commands Parsing
DECLARE_PARSER_TO_JSON(MenuButtonCommands) {
    JsonWrapper ptree;
    if (!object) return ptree;
    return ptree;  // Add any MenuButtonCommands specific fields if needed
}

DECLARE_PARSER_FROM_JSON(MenuButtonCommands) {
    auto result = std::make_shared<MenuButtonCommands>();
    return result;
}

// MenuButton WebApp Parsing
DECLARE_PARSER_TO_JSON(MenuButtonWebApp) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("text", object->text);
    ptree.put("web_app", put(object->webApp));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(MenuButtonWebApp) {
    auto result = std::make_shared<MenuButtonWebApp>();
    result->text = parsePrimitive<std::string>(data, "text");
    result->webApp = parse<WebAppInfo>(data, "web_app");
    return result;
}

// MenuButton Default Parsing
DECLARE_PARSER_TO_JSON(MenuButtonDefault) {
    JsonWrapper ptree;
    if (!object) return ptree;
    return ptree;  // Add any MenuButtonDefault specific fields if needed
}

DECLARE_PARSER_FROM_JSON(MenuButtonDefault) {
    auto result = std::make_shared<MenuButtonDefault>();
    return result;
}

// ChatBoostSourcePremium Parsing
DECLARE_PARSER_TO_JSON(ChatBoostSourcePremium) {
    JsonWrapper ptree;
    if (!object) return ptree;
    return ptree;  // Add specific fields for ChatBoostSourcePremium if any
}

DECLARE_PARSER_FROM_JSON(ChatBoostSourcePremium) {
    auto result = std::make_shared<ChatBoostSourcePremium>();
    return result;
}

// ChatBoostSourceGiftCode Parsing
DECLARE_PARSER_TO_JSON(ChatBoostSourceGiftCode) {
    JsonWrapper ptree;
    if (!object) return ptree;
    return ptree;  // Add specific fields for ChatBoostSourceGiftCode if any
}

DECLARE_PARSER_FROM_JSON(ChatBoostSourceGiftCode) {
    auto result = std::make_shared<ChatBoostSourceGiftCode>();
    return result;
}

// ChatBoostSourceGiveaway Parsing
DECLARE_PARSER_TO_JSON(ChatBoostSourceGiveaway) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("giveaway_message_id", object->giveawayMessageId);
    ptree.put("is_unclaimed", object->isUnclaimed);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoostSourceGiveaway) {
    auto result = std::make_shared<ChatBoostSourceGiveaway>();
    result->giveawayMessageId =
        parsePrimitive<std::int32_t>(data, "giveaway_message_id");
    result->isUnclaimed = parsePrimitive<bool>(data, "is_unclaimed");
    return result;
}

// BusinessConnection Parsing
DECLARE_PARSER_TO_JSON(BusinessConnection) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("id", object->id);
    ptree.put("user", put(object->user));
    ptree.put("user_chat_id", object->userChatId);
    ptree.put("date", object->date);
    ptree.put("can_reply", object->canReply);
    ptree.put("is_enabled", object->isEnabled);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BusinessConnection) {
    auto result = std::make_shared<BusinessConnection>();
    result->id = parsePrimitive<std::string>(data, "id");
    result->user = parse<User>(data, "user");
    result->userChatId = parsePrimitive<std::int64_t>(data, "user_chat_id");
    result->date = parsePrimitive<std::uint32_t>(data, "date");
    result->canReply = parsePrimitive<bool>(data, "can_reply");
    result->isEnabled = parsePrimitive<bool>(data, "is_enabled");
    return result;
}

// BusinessMessagesDeleted Parsing
DECLARE_PARSER_TO_JSON(BusinessMessagesDeleted) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("business_connection_id", object->businessConnectionId);
    ptree.put("chat", put(object->chat));
    ptree.put("message_ids", put(object->messageIds));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BusinessMessagesDeleted) {
    auto result = std::make_shared<BusinessMessagesDeleted>();
    result->businessConnectionId =
        parsePrimitive<std::string>(data, "business_connection_id");
    result->chat = parse<Chat>(data, "chat");
    result->messageIds = parsePrimitiveArray<std::int32_t>(data, "message_ids");
    return result;
}

// ResponseParameters Parsing
DECLARE_PARSER_TO_JSON(ResponseParameters) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("migrate_to_chat_id", object->migrateToChatId);
    ptree.put("retry_after", object->retryAfter);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ResponseParameters) {
    auto result = std::make_shared<ResponseParameters>();
    result->migrateToChatId =
        parsePrimitive<std::int64_t>(data, "migrate_to_chat_id");
    result->retryAfter = parsePrimitive<std::int32_t>(data, "retry_after");
    return result;
}

// InputMedia Parsing
DECLARE_PARSER_TO_JSON(InputMedia) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("type", object->type);
    ptree.put("media", object->media);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));

    if (object->type == InputMediaPhoto::TYPE) {
        ptree += put<InputMediaPhoto>(object);
    } else if (object->type == InputMediaVideo::TYPE) {
        ptree += put<InputMediaVideo>(object);
    } else if (object->type == InputMediaAnimation::TYPE) {
        ptree += put<InputMediaAnimation>(object);
    } else if (object->type == InputMediaAudio::TYPE) {
        ptree += put<InputMediaAudio>(object);
    } else if (object->type == InputMediaDocument::TYPE) {
        ptree += put<InputMediaDocument>(object);
    } else {
        throw invalidType("InputMedia", object->type);
    }

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMedia) {
    auto type = parsePrimitive<std::string>(data, "type");
    InputMedia::Ptr result;

    if (type == InputMediaPhoto::TYPE) {
        result = parse<InputMediaPhoto>(data);
    } else if (type == InputMediaVideo::TYPE) {
        result = parse<InputMediaVideo>(data);
    } else if (type == InputMediaAnimation::TYPE) {
        result = parse<InputMediaAnimation>(data);
    } else if (type == InputMediaAudio::TYPE) {
        result = parse<InputMediaAudio>(data);
    } else if (type == InputMediaDocument::TYPE) {
        result = parse<InputMediaDocument>(data);
    } else {
        throw invalidType("InputMedia", type);
    }

    result->type = parsePrimitive<std::string>(data, "type");
    result->media = parsePrimitive<std::string>(data, "media");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");

    return result;
}

// InputMediaPhoto Parsing
DECLARE_PARSER_TO_JSON(InputMediaPhoto) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("has_spoiler", object->hasSpoiler);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMediaPhoto) {
    auto result = std::make_shared<InputMediaPhoto>();
    result->hasSpoiler = parsePrimitive<bool>(data, "has_spoiler");
    return result;
}

// InputMediaVideo Parsing
DECLARE_PARSER_TO_JSON(InputMediaVideo) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("thumbnail", object->thumbnail);
    ptree.put("width", object->width);
    ptree.put("height", object->height);
    ptree.put("duration", object->duration);
    ptree.put("supports_streaming", object->supportsStreaming);
    ptree.put("has_spoiler", object->hasSpoiler);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMediaVideo) {
    auto result = std::make_shared<InputMediaVideo>();
    result->thumbnail = parsePrimitive<std::string>(data, "thumbnail");
    result->width = parsePrimitive<std::int32_t>(data, "width");
    result->height = parsePrimitive<std::int32_t>(data, "height");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->supportsStreaming =
        parsePrimitive<bool>(data, "supports_streaming");
    result->hasSpoiler = parsePrimitive<bool>(data, "has_spoiler");
    return result;
}

// InputMediaAnimation Parsing
DECLARE_PARSER_TO_JSON(InputMediaAnimation) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("thumbnail", object->thumbnail);
    ptree.put("width", object->width);
    ptree.put("height", object->height);
    ptree.put("duration", object->duration);
    ptree.put("has_spoiler", object->hasSpoiler);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMediaAnimation) {
    auto result = std::make_shared<InputMediaAnimation>();
    result->thumbnail = parsePrimitive<std::string>(data, "thumbnail");
    result->width = parsePrimitive<std::int32_t>(data, "width");
    result->height = parsePrimitive<std::int32_t>(data, "height");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->hasSpoiler = parsePrimitive<bool>(data, "has_spoiler");
    return result;
}

// InputMediaAudio Parsing
DECLARE_PARSER_TO_JSON(InputMediaAudio) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("thumbnail", object->thumbnail);
    ptree.put("duration", object->duration);
    ptree.put("performer", object->performer);
    ptree.put("title", object->title);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMediaAudio) {
    auto result = std::make_shared<InputMediaAudio>();
    result->thumbnail = parsePrimitive<std::string>(data, "thumbnail");
    result->duration = parsePrimitive<std::int32_t>(data, "duration");
    result->performer = parsePrimitive<std::string>(data, "performer");
    result->title = parsePrimitive<std::string>(data, "title");
    return result;
}

// InputMediaDocument
DECLARE_PARSER_FROM_JSON(InputMediaDocument) {
    auto result = std::make_shared<InputMediaDocument>();
    result->thumbnail = parsePrimitive<std::string>(data, "thumbnail");
    result->disableContentTypeDetection =
        parsePrimitive<bool>(data, "disable_content_type_detection");
    return result;
}

DECLARE_PARSER_TO_JSON(InputMediaDocument) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("thumbnail", object->thumbnail);
    ptree.put("disable_content_type_detection",
              object->disableContentTypeDetection);
    return ptree;
}

// Sticker
DECLARE_PARSER_FROM_JSON(Sticker) {
    auto result = std::make_shared<Sticker>();
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    std::string type = parsePrimitive<std::string>(data, "type");
    if (type == "regular") {
        result->type = Sticker::Type::Regular;
    } else if (type == "mask") {
        result->type = Sticker::Type::Mask;
    } else if (type == "custom_emoji") {
        result->type = Sticker::Type::CustomEmoji;
    }
    result->width = parsePrimitive<std::int32_t>(data, "width");
    result->height = parsePrimitive<std::int32_t>(data, "height");
    result->isAnimated = parsePrimitive<bool>(data, "is_animated");
    result->isVideo = parsePrimitive<bool>(data, "is_video");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    result->emoji = parsePrimitive<std::string>(data, "emoji");
    result->setName = parsePrimitive<std::string>(data, "set_name");
    result->premiumAnimation = parse<File>(data, "premium_animation");
    result->maskPosition = parse<MaskPosition>(data, "mask_position");
    result->customEmojiId =
        parsePrimitive<std::string>(data, "custom_emoji_id");
    result->needsRepainting =
        parsePrimitive<bool>(data, "needs_repainting", true);
    result->fileSize = parsePrimitive<std::int32_t>(data, "file_size");
    return result;
}

DECLARE_PARSER_TO_JSON(Sticker) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_id", object->fileId);
    ptree.put("file_unique_id", object->fileUniqueId);
    if (object->type == Sticker::Type::Regular) {
        ptree.put("type", "regular");
    } else if (object->type == Sticker::Type::Mask) {
        ptree.put("type", "mask");
    } else if (object->type == Sticker::Type::CustomEmoji) {
        ptree.put("type", "custom_emoji");
    }
    ptree.put("width", object->width);
    ptree.put("height", object->height);
    ptree.put("is_animated", object->isAnimated);
    ptree.put("is_video", object->isVideo);
    ptree.put("thumbnail", put(object->thumbnail));
    ptree.put("emoji", object->emoji);
    ptree.put("set_name", object->setName);
    ptree.put("premium_animation", put(object->premiumAnimation));
    ptree.put("mask_position", put(object->maskPosition));
    ptree.put("custom_emoji_id", object->customEmojiId);
    ptree.put("needs_repainting", object->needsRepainting);
    ptree.put("file_size", object->fileSize);
    return ptree;
}

// StickerSet
DECLARE_PARSER_FROM_JSON(StickerSet) {
    auto result = std::make_shared<StickerSet>();
    result->name = parsePrimitive<std::string>(data, "name");
    result->title = parsePrimitive<std::string>(data, "title");
    std::string stickerType = parsePrimitive<std::string>(data, "sticker_type");
    if (stickerType == "regular") {
        result->stickerType = StickerSet::Type::Regular;
    } else if (stickerType == "mask") {
        result->stickerType = StickerSet::Type::Mask;
    } else if (stickerType == "custom_emoji") {
        result->stickerType = StickerSet::Type::CustomEmoji;
    }
    result->stickers = parseArray<Sticker>(data, "stickers");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    return result;
}

// BotCommandScopeDefault
DECLARE_PARSER_FROM_JSON(BotCommandScopeDefault) {
    return std::make_shared<BotCommandScopeDefault>();
}

DECLARE_PARSER_TO_JSON(BotCommandScopeDefault) {
    if (!object) return {};
    return {};
}

// BotCommandScopeAllPrivateChats
DECLARE_PARSER_FROM_JSON(BotCommandScopeAllPrivateChats) {
    return std::make_shared<BotCommandScopeAllPrivateChats>();
}

DECLARE_PARSER_TO_JSON(BotCommandScopeAllPrivateChats) {
    if (!object) return {};
    return {};
}

// BotCommandScopeAllGroupChats
DECLARE_PARSER_FROM_JSON(BotCommandScopeAllGroupChats) {
    return std::make_shared<BotCommandScopeAllGroupChats>();
}

DECLARE_PARSER_TO_JSON(BotCommandScopeAllGroupChats) {
    if (!object) return {};
    return {};
}

// BotCommandScopeAllChatAdministrators
DECLARE_PARSER_FROM_JSON(BotCommandScopeAllChatAdministrators) {
    return std::make_shared<BotCommandScopeAllChatAdministrators>();
}

DECLARE_PARSER_TO_JSON(BotCommandScopeAllChatAdministrators) {
    if (!object) return {};
    return {};
}

DECLARE_PARSER_TO_JSON(StickerSet) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("name", object->name);
    ptree.put("title", object->title);
    if (object->stickerType == StickerSet::Type::Regular) {
        ptree.put("sticker_type", "regular");
    } else if (object->stickerType == StickerSet::Type::Mask) {
        ptree.put("sticker_type", "mask");
    } else if (object->stickerType == StickerSet::Type::CustomEmoji) {
        ptree.put("sticker_type", "custom_emoji");
    }
    ptree.put("stickers", put(object->stickers));
    ptree.put("thumbnail", put(object->thumbnail));
    return ptree;
}

// CallbackQuery
DECLARE_PARSER_FROM_JSON(CallbackQuery) {
    auto result = std::make_shared<CallbackQuery>();
    result->id = parsePrimitive<std::string>(data, "id");
    result->from = parse<User>(data, "from");
    result->message = parse<Message>(data, "message");
    result->inlineMessageId =
        parsePrimitive<std::string>(data, "inline_message_id");
    result->chatInstance = parsePrimitive<std::string>(data, "chat_instance");
    result->data = parsePrimitive<std::string>(data, "data");
    result->gameShortName =
        parsePrimitive<std::string>(data, "game_short_name");
    return result;
}

DECLARE_PARSER_TO_JSON(CallbackQuery) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("id", object->id);
    ptree.put("from", put(object->from));
    ptree.put("message", put(object->message));
    ptree.put("inline_message_id", object->inlineMessageId);
    ptree.put("chat_instance", object->chatInstance);
    ptree.put("data", object->data);
    ptree.put("game_short_name", object->gameShortName);
    return ptree;
}

// MaskPosition
DECLARE_PARSER_FROM_JSON(MaskPosition) {
    auto result = std::make_shared<MaskPosition>();
    result->point = parsePrimitive<std::string>(data, "point");
    result->xShift = (float)parsePrimitive<double>(data, "x_shift");
    result->yShift = (float)parsePrimitive<double>(data, "y_shift");
    result->scale = (float)parsePrimitive<double>(data, "scale");
    return result;
}

DECLARE_PARSER_TO_JSON(MaskPosition) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("point", object->point);
    ptree.put("x_shift", object->xShift);
    ptree.put("y_shift", object->yShift);
    ptree.put("scale", object->scale);
    return ptree;
}
// Parsing from JSON to InlineQuery
DECLARE_PARSER_FROM_JSON(InlineQuery) {
    auto result = std::make_shared<InlineQuery>();
    result->id = parsePrimitive<std::string>(data, "id");
    result->from = parse<User>(data, "from");
    result->query = parsePrimitive<std::string>(data, "query");
    result->offset = parsePrimitive<std::string>(data, "offset");
    result->chatType = parsePrimitive<std::string>(data, "chat_type");
    result->location = parse<Location>(data, "location");
    return result;
}

// Serializing InlineQuery to JSON
DECLARE_PARSER_TO_JSON(InlineQuery) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("id", object->id);
    ptree.put("from", put(object->from));
    ptree.put("query", object->query);
    ptree.put("offset", object->offset);
    ptree.put("chat_type", object->chatType);
    ptree.put("location", put(object->location));

    return ptree;
}

// Parsing from JSON to InlineKeyboardButton
DECLARE_PARSER_FROM_JSON(InlineKeyboardButton) {
    auto result = std::make_shared<InlineKeyboardButton>();
    result->text = parsePrimitive<std::string>(data, "text");
    result->url = parsePrimitive<std::string>(data, "url");
    result->callbackData = parsePrimitive<std::string>(data, "callback_data");
    result->webApp = parse<WebAppInfo>(data, "web_app");
    result->loginUrl = parse<LoginUrl>(data, "login_url");
    result->switchInlineQuery =
        parsePrimitive<std::string>(data, "switch_inline_query");
    result->switchInlineQueryCurrentChat =
        parsePrimitive<std::string>(data, "switch_inline_query_current_chat");
    result->switchInlineQueryChosenChat = parse<SwitchInlineQueryChosenChat>(
        data, "switch_inline_query_chosen_chat");
    result->callbackGame = parse<CallbackGame>(data, "callback_game");
    result->pay = parsePrimitive<bool>(data, "pay");
    return result;
}

// Serializing InlineKeyboardButton to JSON
DECLARE_PARSER_TO_JSON(InlineKeyboardButton) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("text", object->text);
    ptree.put("url", object->url);
    ptree.put("callback_data", object->callbackData);
    ptree.put("web_app", put(object->webApp));
    ptree.put("login_url", put(object->loginUrl));
    ptree.put("switch_inline_query", object->switchInlineQuery);
    ptree.put("switch_inline_query_current_chat",
              object->switchInlineQueryCurrentChat);
    ptree.put("switch_inline_query_chosen_chat",
              put(object->switchInlineQueryChosenChat));
    ptree.put("callback_game", put(object->callbackGame));
    ptree.put("pay", object->pay);

    return ptree;
}

template <typename T, typename CachedT>
auto put(const InlineQueryResult::Ptr &ptr) {
    if (const auto cached = std::dynamic_pointer_cast<CachedT>(ptr)) {
        return put<typename CachedT::Ptr>(cached);
    } else if (const auto result = std::dynamic_pointer_cast<T>(ptr)) {
        return put<typename T::Ptr>(result);
    } else {
        throw TgBot::TgException("Invalid inline query result type",
                                 TgException::ErrorCode::Internal);
    }
}

DECLARE_PARSER_TO_JSON(InlineQueryResult) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("type", object->type);
    ptree.put("id", object->id);
    ptree.put("reply_markup", put(object->replyMarkup));

    if (object->type == InlineQueryResultArticle::TYPE) {
        ptree += put<InlineQueryResultArticle>(object);
    } else if (object->type == InlineQueryResultAudio::TYPE) {
        ptree +=
            put<InlineQueryResultAudio, InlineQueryResultCachedAudio>(object);
    } else if (object->type == InlineQueryResultContact::TYPE) {
        ptree += put<InlineQueryResultContact>(object);
    } else if (object->type == InlineQueryResultGame::TYPE) {
        ptree += put<InlineQueryResultGame>(object);
    } else if (object->type == InlineQueryResultDocument::TYPE) {
        ptree += put<InlineQueryResultDocument>(object);
    } else if (object->type == InlineQueryResultLocation::TYPE) {
        ptree += put<InlineQueryResultLocation>(object);
    } else if (object->type == InlineQueryResultVenue::TYPE) {
        ptree += put<InlineQueryResultVenue>(object);
    } else if (object->type == InlineQueryResultVoice::TYPE) {
        ptree +=
            put<InlineQueryResultVoice, InlineQueryResultCachedVoice>(object);
    } else if (object->type == InlineQueryResultPhoto::TYPE) {
        ptree +=
            put<InlineQueryResultPhoto, InlineQueryResultCachedPhoto>(object);
    } else if (object->type == InlineQueryResultGif::TYPE) {
        ptree += put<InlineQueryResultGif, InlineQueryResultCachedGif>(object);
    } else if (object->type == InlineQueryResultMpeg4Gif::TYPE) {
        ptree +=
            put<InlineQueryResultMpeg4Gif, InlineQueryResultCachedMpeg4Gif>(
                object);
    } else if (object->type == InlineQueryResultVideo::TYPE) {
        ptree +=
            put<InlineQueryResultVideo, InlineQueryResultCachedVideo>(object);
    } else if (object->type == InlineQueryResultCachedSticker::TYPE) {
        ptree += put<InlineQueryResultCachedSticker>(object);
    } else {
        throw invalidType("InlineQueryResult", object->type);
    }
    return ptree;
}
// Parsing from JSON to InputSticker
DECLARE_PARSER_FROM_JSON(InputSticker) {
    auto result = std::make_shared<InputSticker>();
    result->sticker = parsePrimitive<std::string>(data, "sticker");
    result->format = parsePrimitive<std::string>(data, "format");
    result->emojiList = parsePrimitiveArray<std::string>(data, "emoji_list");
    result->maskPosition = parse<MaskPosition>(data, "mask_position");
    result->keywords = parsePrimitiveArray<std::string>(data, "keywords");
    return result;
}

// Serializing InputSticker to JSON
DECLARE_PARSER_TO_JSON(InputSticker) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("sticker", object->sticker);
    ptree.put("format", object->format);
    ptree.put("emoji_list", put(object->emojiList));
    ptree.put("mask_position", put(object->maskPosition));
    ptree.put("keywords", put(object->keywords));

    return ptree;
}
// Parsing from JSON to SwitchInlineQueryChosenChat
DECLARE_PARSER_FROM_JSON(SwitchInlineQueryChosenChat) {
    auto result = std::make_shared<SwitchInlineQueryChosenChat>();
    result->query = parsePrimitive<std::string>(data, "query");
    result->allowUserChats = parsePrimitive<bool>(data, "allow_user_chats");
    result->allowBotChats = parsePrimitive<bool>(data, "allow_bot_chats");
    result->allowGroupChats = parsePrimitive<bool>(data, "allow_group_chats");
    result->allowChannelChats =
        parsePrimitive<bool>(data, "allow_channel_chats");
    return result;
}

// Serializing SwitchInlineQueryChosenChat to JSON
DECLARE_PARSER_TO_JSON(SwitchInlineQueryChosenChat) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("query", object->query);
    ptree.put("allow_user_chats", object->allowUserChats);
    ptree.put("allow_bot_chats", object->allowBotChats);
    ptree.put("allow_group_chats", object->allowGroupChats);
    ptree.put("allow_channel_chats", object->allowChannelChats);

    return ptree;
}

// Parsing from JSON to LoginUrl
DECLARE_PARSER_FROM_JSON(LoginUrl) {
    auto result = std::make_shared<LoginUrl>();
    result->url = parsePrimitive<std::string>(data, "url");
    result->forwardText = parsePrimitive<std::string>(data, "forward_text");
    result->botUsername = parsePrimitive<std::string>(data, "bot_username");
    result->requestWriteAccess =
        parsePrimitive<bool>(data, "request_write_access");
    return result;
}

// Serializing LoginUrl to JSON
DECLARE_PARSER_TO_JSON(LoginUrl) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("url", object->url);
    ptree.put("forward_text", object->forwardText);
    ptree.put("bot_username", object->botUsername);
    ptree.put("request_write_access", object->requestWriteAccess);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultsButton) {
    auto result = std::make_shared<InlineQueryResultsButton>();
    result->text = parsePrimitive<std::string>(data, "text");
    result->webApp = parse<WebAppInfo>(data, "web_app");
    result->startParameter =
        parsePrimitive<std::string>(data, "start_parameter");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultsButton) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("text", object->text);
    ptree.put("web_app", put(object->webApp));
    ptree.put("start_parameter", object->startParameter);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultArticle) {
    auto result = std::make_shared<InlineQueryResultArticle>();
    result->title = parsePrimitive<std::string>(data, "title");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    result->url = parsePrimitive<std::string>(data, "url");
    result->hideUrl = parsePrimitive<bool>(data, "hide_url");
    result->description = parsePrimitive<std::string>(data, "description");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->thumbnailWidth =
        parsePrimitive<std::int32_t>(data, "thumbnail_width");
    result->thumbnailHeight =
        parsePrimitive<std::int32_t>(data, "thumbnail_height");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultArticle) {
    if (!object) return {};

    JsonWrapper ptree;
    ptree.put("title", object->title);
    ptree.put("input_message_content", put(object->inputMessageContent));
    ptree.put("url", object->url);
    ptree.put("hide_url", object->hideUrl);
    ptree.put("description", object->description);
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("thumbnail_width", object->thumbnailWidth);
    ptree.put("thumbnail_height", object->thumbnailHeight);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultPhoto) {
    auto result = std::make_shared<InlineQueryResultPhoto>();
    result->photoUrl = parsePrimitive<std::string>(data, "photo_url");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->photoWidth = parsePrimitive<std::int32_t>(data, "photo_width");
    result->photoHeight = parsePrimitive<std::int32_t>(data, "photo_height");
    result->title = parsePrimitive<std::string>(data, "title");
    result->description = parsePrimitive<std::string>(data, "description");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultPhoto) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("photo_url", object->photoUrl);
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("photo_width", object->photoWidth);
    ptree.put("photo_height", object->photoHeight);
    ptree.put("title", object->title);
    ptree.put("description", object->description);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultGif) {
    auto result = std::make_shared<InlineQueryResultGif>();
    result->gifUrl = parsePrimitive<std::string>(data, "gif_url");
    result->gifWidth = parsePrimitive<std::int32_t>(data, "gif_width");
    result->gifHeight = parsePrimitive<std::int32_t>(data, "gif_height");
    result->gifDuration = parsePrimitive<std::int32_t>(data, "gif_duration");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->thumbnailMimeType =
        parsePrimitive<std::string>(data, "thumbnail_mime_type");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultGif) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("gif_url", object->gifUrl);
    ptree.put("gif_width", object->gifWidth);
    ptree.put("gif_height", object->gifHeight);
    ptree.put("gif_duration", object->gifDuration);
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("thumbnail_mime_type", object->thumbnailMimeType);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultMpeg4Gif) {
    auto result = std::make_shared<InlineQueryResultMpeg4Gif>();
    result->mpeg4Url = parsePrimitive<std::string>(data, "mpeg4_url");
    result->mpeg4Width = parsePrimitive<std::int32_t>(data, "mpeg4_width");
    result->mpeg4Height = parsePrimitive<std::int32_t>(data, "mpeg4_height");
    result->mpeg4Duration =
        parsePrimitive<std::int32_t>(data, "mpeg4_duration");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->thumbnailMimeType =
        parsePrimitive<std::string>(data, "thumbnail_mime_type");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultMpeg4Gif) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("mpeg4_url", object->mpeg4Url);
    ptree.put("mpeg4_width", object->mpeg4Width);
    ptree.put("mpeg4_height", object->mpeg4Height);
    ptree.put("mpeg4_duration", object->mpeg4Duration);
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("thumbnail_mime_type", object->thumbnailMimeType);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultVideo) {
    auto result = std::make_shared<InlineQueryResultVideo>();
    result->videoUrl = parsePrimitive<std::string>(data, "video_url");
    result->mimeType = parsePrimitive<std::string>(data, "mime_type");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->videoWidth = parsePrimitive<std::int32_t>(data, "video_width");
    result->videoHeight = parsePrimitive<std::int32_t>(data, "video_height");
    result->videoDuration =
        parsePrimitive<std::int32_t>(data, "video_duration");
    result->description = parsePrimitive<std::string>(data, "description");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVideo) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("video_url", object->videoUrl);
    ptree.put("mime_type", object->mimeType);
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("video_width", object->videoWidth);
    ptree.put("video_height", object->videoHeight);
    ptree.put("video_duration", object->videoDuration);
    ptree.put("description", object->description);
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultAudio) {
    auto result = std::make_shared<InlineQueryResultAudio>();
    result->audioUrl = parsePrimitive<std::string>(data, "audio_url");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->performer = parsePrimitive<std::string>(data, "performer");
    result->audioDuration =
        parsePrimitive<std::int32_t>(data, "audio_duration");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultAudio) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("audio_url", object->audioUrl);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("performer", object->performer);
    ptree.put("audio_duration", object->audioDuration);
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultVoice) {
    auto result = std::make_shared<InlineQueryResultVoice>();
    result->voiceUrl = parsePrimitive<std::string>(data, "voice_url");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->voiceDuration =
        parsePrimitive<std::int32_t>(data, "voice_duration");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVoice) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("voice_url", object->voiceUrl);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("voice_duration", object->voiceDuration);
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultDocument) {
    auto result = std::make_shared<InlineQueryResultDocument>();
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->documentUrl = parsePrimitive<std::string>(data, "document_url");
    result->mimeType = parsePrimitive<std::string>(data, "mime_type");
    result->description = parsePrimitive<std::string>(data, "description");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->thumbnailWidth =
        parsePrimitive<std::int32_t>(data, "thumbnail_width");
    result->thumbnailHeight =
        parsePrimitive<std::int32_t>(data, "thumbnail_height");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultDocument) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("document_url", object->documentUrl);
    ptree.put("mime_type", object->mimeType);
    ptree.put("description", object->description);
    ptree.put("input_message_content", put(object->inputMessageContent));
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("thumbnail_width", object->thumbnailWidth);
    ptree.put("thumbnail_height", object->thumbnailHeight);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultLocation) {
    auto result = std::make_shared<InlineQueryResultLocation>();
    result->latitude = (float)parsePrimitive<double>(data, "latitude");
    result->longitude = (float)parsePrimitive<double>(data, "longitude");
    result->title = parsePrimitive<std::string>(data, "title");
    result->horizontalAccuracy =
        (float)parsePrimitive<double>(data, "horizontal_accuracy");
    result->livePeriod = parsePrimitive<std::int32_t>(data, "live_period");
    result->heading = parsePrimitive<std::int32_t>(data, "heading");
    result->proximityAlertRadius =
        parsePrimitive<std::int32_t>(data, "proximity_alert_radius");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->thumbnailWidth =
        parsePrimitive<std::int32_t>(data, "thumbnail_width");
    result->thumbnailHeight =
        parsePrimitive<std::int32_t>(data, "thumbnail_height");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultLocation) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("latitude", object->latitude);
    ptree.put("longitude", object->longitude);
    ptree.put("title", object->title);
    ptree.put("horizontal_accuracy", object->horizontalAccuracy);
    ptree.put("live_period", object->livePeriod);
    ptree.put("heading", object->heading);
    ptree.put("proximity_alert_radius", object->proximityAlertRadius);
    ptree.put("input_message_content", put(object->inputMessageContent));
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("thumbnail_width", object->thumbnailWidth);
    ptree.put("thumbnail_height", object->thumbnailHeight);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultVenue) {
    auto result = std::make_shared<InlineQueryResultVenue>();
    result->latitude = (float)parsePrimitive<double>(data, "latitude");
    result->longitude = (float)parsePrimitive<double>(data, "longitude");
    result->title = parsePrimitive<std::string>(data, "title");
    result->address = parsePrimitive<std::string>(data, "address");
    result->foursquareId = parsePrimitive<std::string>(data, "foursquare_id");
    result->foursquareType =
        parsePrimitive<std::string>(data, "foursquare_type");
    result->googlePlaceId =
        parsePrimitive<std::string>(data, "google_place_id");
    result->googlePlaceType =
        parsePrimitive<std::string>(data, "google_place_type");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->thumbnailWidth =
        parsePrimitive<std::int32_t>(data, "thumbnail_width");
    result->thumbnailHeight =
        parsePrimitive<std::int32_t>(data, "thumbnail_height");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVenue) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("latitude", object->latitude);
    ptree.put("longitude", object->longitude);
    ptree.put("title", object->title);
    ptree.put("address", object->address);
    ptree.put("foursquare_id", object->foursquareId);
    ptree.put("foursquare_type", object->foursquareType);
    ptree.put("google_place_id", object->googlePlaceId);
    ptree.put("google_place_type", object->googlePlaceType);
    ptree.put("input_message_content", put(object->inputMessageContent));
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("thumbnail_width", object->thumbnailWidth);
    ptree.put("thumbnail_height", object->thumbnailHeight);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultContact) {
    auto result = std::make_shared<InlineQueryResultContact>();
    result->phoneNumber = parsePrimitive<std::string>(data, "phone_number");
    result->firstName = parsePrimitive<std::string>(data, "first_name");
    result->lastName = parsePrimitive<std::string>(data, "last_name");
    result->vcard = parsePrimitive<std::string>(data, "vcard");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    result->thumbnailUrl = parsePrimitive<std::string>(data, "thumbnail_url");
    result->thumbnailWidth =
        parsePrimitive<std::int32_t>(data, "thumbnail_width");
    result->thumbnailHeight =
        parsePrimitive<std::int32_t>(data, "thumbnail_height");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultContact) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("phone_number", object->phoneNumber);
    ptree.put("first_name", object->firstName);
    ptree.put("last_name", object->lastName);
    ptree.put("vcard", object->vcard);
    ptree.put("input_message_content", put(object->inputMessageContent));
    ptree.put("thumbnail_url", object->thumbnailUrl);
    ptree.put("thumbnail_width", object->thumbnailWidth);
    ptree.put("thumbnail_height", object->thumbnailHeight);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultGame) {
    auto result = std::make_shared<InlineQueryResultGame>();
    result->gameShortName =
        parsePrimitive<std::string>(data, "game_short_name");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultGame) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("game_short_name", object->gameShortName);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedPhoto) {
    auto result = std::make_shared<InlineQueryResultCachedPhoto>();
    result->photoFileId = parsePrimitive<std::string>(data, "photo_file_id");
    result->title = parsePrimitive<std::string>(data, "title");
    result->description = parsePrimitive<std::string>(data, "description");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedPhoto) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("photo_file_id", object->photoFileId);
    ptree.put("title", object->title);
    ptree.put("description", object->description);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedGif) {
    auto result = std::make_shared<InlineQueryResultCachedGif>();
    result->gifFileId = parsePrimitive<std::string>(data, "gif_file_id");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedGif) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("gif_file_id", object->gifFileId);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedMpeg4Gif) {
    auto result = std::make_shared<InlineQueryResultCachedMpeg4Gif>();
    result->mpeg4FileId = parsePrimitive<std::string>(data, "mpeg4_file_id");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedMpeg4Gif) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("mpeg4_file_id", object->mpeg4FileId);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}
DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedSticker) {
    auto result = std::make_shared<InlineQueryResultCachedSticker>();
    result->stickerFileId =
        parsePrimitive<std::string>(data, "sticker_file_id");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedSticker) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("sticker_file_id", object->stickerFileId);
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedDocument) {
    auto result = std::make_shared<InlineQueryResultCachedDocument>();
    result->title = parsePrimitive<std::string>(data, "title");
    result->documentFileId =
        parsePrimitive<std::string>(data, "document_file_id");
    result->description = parsePrimitive<std::string>(data, "description");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedDocument) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("title", object->title);
    ptree.put("document_file_id", object->documentFileId);
    ptree.put("description", object->description);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedVideo) {
    auto result = std::make_shared<InlineQueryResultCachedVideo>();
    result->videoFileId = parsePrimitive<std::string>(data, "video_file_id");
    result->title = parsePrimitive<std::string>(data, "title");
    result->description = parsePrimitive<std::string>(data, "description");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedVideo) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("video_file_id", object->videoFileId);
    ptree.put("title", object->title);
    ptree.put("description", object->description);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedVoice) {
    auto result = std::make_shared<InlineQueryResultCachedVoice>();
    result->voiceFileId = parsePrimitive<std::string>(data, "voice_file_id");
    result->title = parsePrimitive<std::string>(data, "title");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedVoice) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("voice_file_id", object->voiceFileId);
    ptree.put("title", object->title);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedAudio) {
    auto result = std::make_shared<InlineQueryResultCachedAudio>();
    result->audioFileId = parsePrimitive<std::string>(data, "audio_file_id");
    result->caption = parsePrimitive<std::string>(data, "caption");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedAudio) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("audio_file_id", object->audioFileId);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMessageContent) {
    InputMessageContent::Ptr result;

    std::string messageText = parsePrimitive<std::string>(data, "message_text");
    float latitude = (float)parsePrimitive<double>(data, "latitude", 1000);
    std::string address = parsePrimitive<std::string>(data, "address");
    std::string phoneNumber = parsePrimitive<std::string>(data, "phone_number");
    std::string description = parsePrimitive<std::string>(data, "description");

    if (!messageText.empty()) {
        result = parse<InputTextMessageContent>(data);
    } else if (!address.empty()) {
        result = parse<InputVenueMessageContent>(data);
    } else if (latitude != 1000) {
        result = parse<InputLocationMessageContent>(data);
    } else if (!phoneNumber.empty()) {
        result = parse<InputContactMessageContent>(data);
    } else if (!description.empty()) {
        result = parse<InputInvoiceMessageContent>(data);
    } else {
        result = std::make_shared<InputMessageContent>();
    }

    return result;
}

DECLARE_PARSER_TO_JSON(InputMessageContent) {
    if (!object) {
        return {};
    }
    JsonWrapper ptree;

    if (object->type == InputTextMessageContent::TYPE) {
        ptree = put<InputTextMessageContent>(object);
    } else if (object->type == InputLocationMessageContent::TYPE) {
        ptree = put<InputLocationMessageContent>(object);
    } else if (object->type == InputVenueMessageContent::TYPE) {
        ptree = put<InputVenueMessageContent>(object);
    } else if (object->type == InputContactMessageContent::TYPE) {
        ptree = put<InputContactMessageContent>(object);
    } else if (object->type == InputInvoiceMessageContent::TYPE) {
        ptree = put<InputInvoiceMessageContent>(object);
    } else {
        throw invalidType("InputMessageContent", object->type);
    }

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputTextMessageContent) {
    auto result = std::make_shared<InputTextMessageContent>();
    result->messageText = parsePrimitive<std::string>(data, "message_text");
    result->parseMode = parsePrimitive<std::string>(data, "parse_mode");
    result->entities = parseArray<MessageEntity>(data, "entities");
    result->linkPreviewOptions =
        parse<LinkPreviewOptions>(data, "link_preview_options");
    return result;
}

DECLARE_PARSER_TO_JSON(InputTextMessageContent) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("message_text", object->messageText);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("entities", put(object->entities));
    ptree.put("link_preview_options", put(object->linkPreviewOptions));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputLocationMessageContent) {
    auto result = std::make_shared<InputLocationMessageContent>();
    result->latitude = (float)parsePrimitive<double>(data, "latitude");
    result->longitude = (float)parsePrimitive<double>(data, "longitude");
    result->horizontalAccuracy =
        (float)parsePrimitive<double>(data, "horizontal_accuracy");
    result->livePeriod = parsePrimitive<std::int32_t>(data, "live_period");
    result->heading = parsePrimitive<std::int32_t>(data, "heading");
    result->proximityAlertRadius =
        parsePrimitive<std::int32_t>(data, "proximity_alert_radius");
    return result;
}

DECLARE_PARSER_TO_JSON(InputLocationMessageContent) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("latitude", object->latitude);
    ptree.put("longitude", object->longitude);
    ptree.put("horizontal_accuracy", object->horizontalAccuracy);
    ptree.put("live_period", object->livePeriod);
    ptree.put("heading", object->heading);
    ptree.put("proximity_alert_radius", object->proximityAlertRadius);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputVenueMessageContent) {
    auto result = std::make_shared<InputVenueMessageContent>();
    result->latitude = (float)parsePrimitive<double>(data, "latitude");
    result->longitude = (float)parsePrimitive<double>(data, "longitude");
    result->title = parsePrimitive<std::string>(data, "title");
    result->address = parsePrimitive<std::string>(data, "address");
    result->foursquareId = parsePrimitive<std::string>(data, "foursquare_id");
    result->foursquareType =
        parsePrimitive<std::string>(data, "foursquare_type");
    result->googlePlaceId =
        parsePrimitive<std::string>(data, "google_place_id");
    result->googlePlaceType =
        parsePrimitive<std::string>(data, "google_place_type");
    return result;
}

DECLARE_PARSER_TO_JSON(InputVenueMessageContent) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("latitude", object->latitude);
    ptree.put("longitude", object->longitude);
    ptree.put("title", object->title);
    ptree.put("address", object->address);
    ptree.put("foursquare_id", object->foursquareId);
    ptree.put("foursquare_type", object->foursquareType);
    ptree.put("google_place_id", object->googlePlaceId);
    ptree.put("google_place_type", object->googlePlaceType);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputContactMessageContent) {
    auto result = std::make_shared<InputContactMessageContent>();
    result->phoneNumber = parsePrimitive<std::string>(data, "phone_number");
    result->firstName = parsePrimitive<std::string>(data, "first_name");
    result->lastName = parsePrimitive<std::string>(data, "last_name");
    result->vcard = parsePrimitive<std::string>(data, "vcard");
    return result;
}

DECLARE_PARSER_TO_JSON(InputContactMessageContent) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("phone_number", object->phoneNumber);
    ptree.put("first_name", object->firstName);
    ptree.put("last_name", object->lastName);
    ptree.put("vcard", object->vcard);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputInvoiceMessageContent) {
    auto result = std::make_shared<InputInvoiceMessageContent>();
    result->title = parsePrimitive<std::string>(data, "title");
    result->description = parsePrimitive<std::string>(data, "description");
    result->payload = parsePrimitive<std::string>(data, "payload");
    result->providerToken = parsePrimitive<std::string>(data, "provider_token");
    result->currency = parsePrimitive<std::string>(data, "currency");
    result->prices = parseArray<LabeledPrice>(data, "prices");
    result->maxTipAmount = parsePrimitive<std::int32_t>(data, "max_tip_amount");
    result->suggestedTipAmounts =
        parsePrimitiveArray<std::int32_t>(data, "suggested_tip_amounts");
    result->providerData = parsePrimitive<std::string>(data, "provider_data");
    result->photoUrl = parsePrimitive<std::string>(data, "photo_url");
    result->photoSize = parsePrimitive<std::int32_t>(data, "photo_size");
    result->photoWidth = parsePrimitive<std::int32_t>(data, "photo_width");
    result->photoHeight = parsePrimitive<std::int32_t>(data, "photo_height");
    result->needName = parsePrimitive<bool>(data, "need_name");
    result->needPhoneNumber = parsePrimitive<bool>(data, "need_phone_number");
    result->needEmail = parsePrimitive<bool>(data, "need_email");
    result->needShippingAddress =
        parsePrimitive<bool>(data, "need_shipping_address");
    result->sendPhoneNumberToProvider =
        parsePrimitive<bool>(data, "send_phone_number_to_provider");
    result->sendEmailToProvider =
        parsePrimitive<bool>(data, "send_email_to_provider");
    result->isFlexible = parsePrimitive<bool>(data, "is_flexible");
    return result;
}

DECLARE_PARSER_TO_JSON(InputInvoiceMessageContent) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("title", object->title);
    ptree.put("description", object->description);
    ptree.put("payload", object->payload);
    ptree.put("provider_token", object->providerToken);
    ptree.put("currency", object->currency);
    ptree.put("prices", put(object->prices));
    ptree.put("max_tip_amount", object->maxTipAmount);
    ptree.put("suggested_tip_amounts", put(object->suggestedTipAmounts));
    ptree.put("provider_data", object->providerData);
    ptree.put("photo_url", object->photoUrl);
    ptree.put("photo_size", object->photoSize);
    ptree.put("photo_width", object->photoWidth);
    ptree.put("photo_height", object->photoHeight);
    ptree.put("need_name", object->needName);
    ptree.put("need_phone_number", object->needPhoneNumber);
    ptree.put("need_email", object->needEmail);
    ptree.put("need_shipping_address", object->needShippingAddress);
    ptree.put("send_phone_number_to_provider",
              object->sendPhoneNumberToProvider);
    ptree.put("send_email_to_provider", object->sendEmailToProvider);
    ptree.put("is_flexible", object->isFlexible);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChosenInlineResult) {
    auto result = std::make_shared<ChosenInlineResult>();
    result->resultId = parsePrimitive<std::string>(data, "result_id");
    result->from = parse<User>(data, "from");
    result->location = parse<Location>(data, "location");
    result->inlineMessageId =
        parsePrimitive<std::string>(data, "inline_message_id");
    result->query = parsePrimitive<std::string>(data, "query");
    return result;
}

DECLARE_PARSER_TO_JSON(ChosenInlineResult) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("result_id", object->resultId);
    ptree.put("from", put(object->from));
    ptree.put("location", put(object->location));
    ptree.put("inline_message_id", object->inlineMessageId);
    ptree.put("query", object->query);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(SentWebAppMessage) {
    auto result = std::make_shared<SentWebAppMessage>();
    result->inlineMessageId =
        parsePrimitive<std::string>(data, "inline_message_id");
    return result;
}

DECLARE_PARSER_TO_JSON(SentWebAppMessage) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("inline_message_id", object->inlineMessageId);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(LabeledPrice) {
    auto result = std::make_shared<LabeledPrice>();
    result->label = parsePrimitive<std::string>(data, "label");
    result->amount = parsePrimitive<std::int32_t>(data, "amount");
    return result;
}

DECLARE_PARSER_TO_JSON(LabeledPrice) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("label", object->label);
    ptree.put("amount", object->amount);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(Invoice) {
    auto result = std::make_shared<Invoice>();
    result->title = parsePrimitive<std::string>(data, "title");
    result->description = parsePrimitive<std::string>(data, "description");
    result->startParameter =
        parsePrimitive<std::string>(data, "start_parameter");
    result->currency = parsePrimitive<std::string>(data, "currency");
    result->totalAmount = parsePrimitive<std::int32_t>(data, "total_amount");
    return result;
}

DECLARE_PARSER_TO_JSON(Invoice) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("title", object->title);
    ptree.put("description", object->description);
    ptree.put("start_parameter", object->startParameter);
    ptree.put("currency", object->currency);
    ptree.put("total_amount", object->totalAmount);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ShippingAddress) {
    auto result = std::make_shared<ShippingAddress>();
    result->countryCode = parsePrimitive<std::string>(data, "country_code");
    result->state = parsePrimitive<std::string>(data, "state");
    result->city = parsePrimitive<std::string>(data, "city");
    result->streetLine1 = parsePrimitive<std::string>(data, "street_line1");
    result->streetLine2 = parsePrimitive<std::string>(data, "street_line2");
    result->postCode = parsePrimitive<std::string>(data, "post_code");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingAddress) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("country_code", object->countryCode);
    ptree.put("state", object->state);
    ptree.put("city", object->city);
    ptree.put("street_line1", object->streetLine1);
    ptree.put("street_line2", object->streetLine2);
    ptree.put("post_code", object->postCode);
    return ptree;
}
// OrderInfo
DECLARE_PARSER_FROM_JSON(OrderInfo) {
    auto result = std::make_shared<OrderInfo>();
    result->name = parsePrimitive<std::string>(data, "name");
    result->phoneNumber = parsePrimitive<std::string>(data, "phone_number");
    result->email = parsePrimitive<std::string>(data, "email");
    result->shippingAddress = parse<ShippingAddress>(data, "shipping_address");
    return result;
}

DECLARE_PARSER_TO_JSON(OrderInfo) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("name", object->name);
    ptree.put("phone_number", object->phoneNumber);
    ptree.put("email", object->email);
    ptree.put("shipping_address", put(object->shippingAddress));
    return ptree;
}

// ShippingOption
DECLARE_PARSER_FROM_JSON(ShippingOption) {
    auto result = std::make_shared<ShippingOption>();
    result->id = parsePrimitive<std::string>(data, "id");
    result->title = parsePrimitive<std::string>(data, "title");
    result->prices = parseArray<LabeledPrice>(data, "prices");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingOption) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("id", object->id);
    ptree.put("title", object->title);
    ptree.put("prices", put(object->prices));
    return ptree;
}

// SuccessfulPayment
DECLARE_PARSER_FROM_JSON(SuccessfulPayment) {
    auto result = std::make_shared<SuccessfulPayment>();
    result->currency = parsePrimitive<std::string>(data, "currency");
    result->totalAmount = parsePrimitive<std::int32_t>(data, "total_amount");
    result->invoicePayload =
        parsePrimitive<std::string>(data, "invoice_payload");
    result->shippingOptionId =
        parsePrimitive<std::string>(data, "shipping_option_id");
    result->orderInfo = parse<OrderInfo>(data, "order_info");
    result->telegramPaymentChargeId =
        parsePrimitive<std::string>(data, "telegram_payment_charge_id");
    result->providerPaymentChargeId =
        parsePrimitive<std::string>(data, "provider_payment_charge_id");
    return result;
}

DECLARE_PARSER_TO_JSON(SuccessfulPayment) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("currency", object->currency);
    ptree.put("total_amount", object->totalAmount);
    ptree.put("invoice_payload", object->invoicePayload);
    ptree.put("shipping_option_id", object->shippingOptionId);
    ptree.put("order_info", put(object->orderInfo));
    ptree.put("telegram_payment_charge_id", object->telegramPaymentChargeId);
    ptree.put("provider_payment_charge_id", object->providerPaymentChargeId);
    return ptree;
}

// ShippingQuery
DECLARE_PARSER_FROM_JSON(ShippingQuery) {
    auto result = std::make_shared<ShippingQuery>();
    result->id = parsePrimitive<std::string>(data, "id");
    result->from = parse<User>(data, "from");
    result->invoicePayload =
        parsePrimitive<std::string>(data, "invoice_payload");
    result->shippingAddress = parse<ShippingAddress>(data, "shipping_address");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingQuery) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("id", object->id);
    ptree.put("from", put(object->from));
    ptree.put("invoice_payload", object->invoicePayload);
    ptree.put("shipping_address", put(object->shippingAddress));
    return ptree;
}

// PreCheckoutQuery
DECLARE_PARSER_FROM_JSON(PreCheckoutQuery) {
    auto result = std::make_shared<PreCheckoutQuery>();
    result->id = parsePrimitive<std::string>(data, "id");
    result->from = parse<User>(data, "from");
    result->currency = parsePrimitive<std::string>(data, "currency");
    result->totalAmount = parsePrimitive<std::int32_t>(data, "total_amount");
    result->invoicePayload =
        parsePrimitive<std::string>(data, "invoice_payload");
    result->shippingOptionId =
        parsePrimitive<std::string>(data, "shipping_option_id");
    result->orderInfo = parse<OrderInfo>(data, "order_info");
    return result;
}

DECLARE_PARSER_TO_JSON(PreCheckoutQuery) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("id", object->id);
    ptree.put("from", put(object->from));
    ptree.put("currency", object->currency);
    ptree.put("total_amount", object->totalAmount);
    ptree.put("invoice_payload", object->invoicePayload);
    ptree.put("shipping_option_id", object->shippingOptionId);
    ptree.put("order_info", put(object->orderInfo));
    return ptree;
}

// PassportData
DECLARE_PARSER_FROM_JSON(PassportData) {
    auto result = std::make_shared<PassportData>();
    result->data = parseArray<EncryptedPassportElement>(data, "data");
    result->credentials = parse<EncryptedCredentials>(data, "credentials");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportData) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("data", put(object->data));
    ptree.put("credentials", put(object->credentials));
    return ptree;
}

// PassportFile
DECLARE_PARSER_FROM_JSON(PassportFile) {
    auto result = std::make_shared<PassportFile>();
    result->fileId = parsePrimitive<std::string>(data, "file_id");
    result->fileUniqueId = parsePrimitive<std::string>(data, "file_unique_id");
    result->fileSize = parsePrimitive<std::int32_t>(data, "file_size");
    result->fileDate = parsePrimitive<std::int32_t>(data, "file_date");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportFile) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_id", object->fileId);
    ptree.put("file_unique_id", object->fileUniqueId);
    ptree.put("file_size", object->fileSize);
    ptree.put("file_date", object->fileDate);
    return ptree;
}

// EncryptedPassportElement
DECLARE_PARSER_FROM_JSON(EncryptedPassportElement) {
    auto result = std::make_shared<EncryptedPassportElement>();
    result->type = parsePrimitive<std::string>(data, "type");
    result->data = parsePrimitive<std::string>(data, "data");
    result->phoneNumber = parsePrimitive<std::string>(data, "phone_number");
    result->email = parsePrimitive<std::string>(data, "email");
    result->files = parseArray<PassportFile>(data, "files");
    result->frontSide = parse<PassportFile>(data, "front_side");
    result->reverseSide = parse<PassportFile>(data, "reverse_side");
    result->selfie = parse<PassportFile>(data, "selfie");
    result->translation = parseArray<PassportFile>(data, "translation");
    result->hash = parsePrimitive<std::string>(data, "hash");
    return result;
}

DECLARE_PARSER_TO_JSON(EncryptedPassportElement) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("type", object->type);
    ptree.put("data", object->data);
    ptree.put("phone_number", object->phoneNumber);
    ptree.put("email", object->email);
    ptree.put("files", put(object->files));
    ptree.put("front_side", put(object->frontSide));
    ptree.put("reverse_side", put(object->reverseSide));
    ptree.put("selfie", put(object->selfie));
    ptree.put("translation", put(object->translation));
    ptree.put("hash", object->hash);
    return ptree;
}

// EncryptedCredentials
DECLARE_PARSER_FROM_JSON(EncryptedCredentials) {
    auto result = std::make_shared<EncryptedCredentials>();
    result->data = parsePrimitive<std::string>(data, "data");
    result->hash = parsePrimitive<std::string>(data, "hash");
    result->secret = parsePrimitive<std::string>(data, "secret");
    return result;
}

DECLARE_PARSER_TO_JSON(EncryptedCredentials) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("data", object->data);
    ptree.put("hash", object->hash);
    ptree.put("secret", object->secret);
    return ptree;
}

// PassportElementError
DECLARE_PARSER_FROM_JSON(PassportElementError) {
    auto source = parsePrimitive<std::string>(data, "source");
    PassportElementError::Ptr result;
    if (source == PassportElementErrorDataField::SOURCE) {
        result = parse<PassportElementErrorDataField>(data);
    } else if (source == PassportElementErrorFrontSide::SOURCE) {
        result = parse<PassportElementErrorFrontSide>(data);
    } else if (source == PassportElementErrorReverseSide::SOURCE) {
        result = parse<PassportElementErrorReverseSide>(data);
    } else if (source == PassportElementErrorSelfie::SOURCE) {
        result = parse<PassportElementErrorSelfie>(data);
    } else {
        return {};
    }
    result->source = parsePrimitive<std::string>(data, "source");
    result->type = parsePrimitive<std::string>(data, "type");
    result->message = parsePrimitive<std::string>(data, "message");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementError) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("source", object->source);
    ptree.put("type", object->type);
    ptree.put("message", object->message);
    return ptree;
}

// PassportElementErrorDataField
DECLARE_PARSER_FROM_JSON(PassportElementErrorDataField) {
    auto result = std::make_shared<PassportElementErrorDataField>();
    result->fieldName = parsePrimitive<std::string>(data, "field_name");
    result->dataHash = parsePrimitive<std::string>(data, "data_hash");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorDataField) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("field_name", object->fieldName);
    ptree.put("data_hash", object->dataHash);
    return ptree;
}

// PassportElementErrorFrontSide
DECLARE_PARSER_FROM_JSON(PassportElementErrorFrontSide) {
    auto result = std::make_shared<PassportElementErrorFrontSide>();
    result->fileHash = parsePrimitive<std::string>(data, "file_hash");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorFrontSide) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorReverseSide
DECLARE_PARSER_FROM_JSON(PassportElementErrorReverseSide) {
    auto result = std::make_shared<PassportElementErrorReverseSide>();
    result->fileHash = parsePrimitive<std::string>(data, "file_hash");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorReverseSide) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorSelfie
DECLARE_PARSER_FROM_JSON(PassportElementErrorSelfie) {
    auto result = std::make_shared<PassportElementErrorSelfie>();
    result->fileHash = parsePrimitive<std::string>(data, "file_hash");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorSelfie) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorFile
DECLARE_PARSER_FROM_JSON(PassportElementErrorFile) {
    auto result = std::make_shared<PassportElementErrorFile>();
    result->fileHash = parsePrimitive<std::string>(data, "file_hash");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorFile) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorFiles
DECLARE_PARSER_FROM_JSON(PassportElementErrorFiles) {
    auto result = std::make_shared<PassportElementErrorFiles>();
    result->fileHashes = parsePrimitiveArray<std::string>(data, "file_hashes");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorFiles) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_hashes", put(object->fileHashes));
    return ptree;
}

// PassportElementErrorTranslationFile
DECLARE_PARSER_FROM_JSON(PassportElementErrorTranslationFile) {
    auto result = std::make_shared<PassportElementErrorTranslationFile>();
    result->fileHash = parsePrimitive<std::string>(data, "file_hash");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorTranslationFile) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorTranslationFiles
DECLARE_PARSER_FROM_JSON(PassportElementErrorTranslationFiles) {
    auto result = std::make_shared<PassportElementErrorTranslationFiles>();
    result->fileHashes = parsePrimitiveArray<std::string>(data, "file_hashes");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorTranslationFiles) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("file_hashes", put(object->fileHashes));
    return ptree;
}

// PassportElementErrorUnspecified
DECLARE_PARSER_FROM_JSON(PassportElementErrorUnspecified) {
    auto result = std::make_shared<PassportElementErrorUnspecified>();
    result->elementHash = parsePrimitive<std::string>(data, "element_hash");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorUnspecified) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("element_hash", object->elementHash);
    return ptree;
}

// Game
DECLARE_PARSER_FROM_JSON(Game) {
    auto result = std::make_shared<Game>();
    result->title = parsePrimitive<std::string>(data, "title");
    result->description = parsePrimitive<std::string>(data, "description");
    result->photo = parseArray<PhotoSize>(data, "photo");
    result->text = parsePrimitive<std::string>(data, "text");
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    result->animation = parse<Animation>(data, "animation");
    return result;
}

DECLARE_PARSER_TO_JSON(Game) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("title", object->title);
    ptree.put("description", object->description);
    ptree.put("photo", put(object->photo));
    ptree.put("text", object->text);
    ptree.put("text_entities", put(object->textEntities));
    ptree.put("animation", put(object->animation));
    return ptree;
}

// CallbackGame
DECLARE_PARSER_FROM_JSON(CallbackGame) {
    auto result = std::make_shared<CallbackGame>();
    return result;
}

DECLARE_PARSER_TO_JSON(CallbackGame) {
    if (!object) return {};
    JsonWrapper ptree;
    return ptree;
}

// GameHighScore
DECLARE_PARSER_FROM_JSON(GameHighScore) {
    auto result = std::make_shared<GameHighScore>();
    result->position = parsePrimitive<std::int32_t>(data, "position");
    result->user = parse<User>(data, "user");
    result->score = parsePrimitive<std::int32_t>(data, "score");
    return result;
}

DECLARE_PARSER_TO_JSON(GameHighScore) {
    if (!object) return {};
    JsonWrapper ptree;
    ptree.put("position", object->position);
    ptree.put("user", put(object->user));
    ptree.put("score", object->score);
    return ptree;
}

// GenericReply
DECLARE_PARSER_FROM_JSON(GenericReply) {
    if (data.isMember("force_reply")) {
        return parse<ForceReply>(data);
    } else if (data.isMember("remove_keyboard")) {
        return parse<ReplyKeyboardRemove>(data);
    } else if (data.isMember("keyboard")) {
        return parse<ReplyKeyboardMarkup>(data);
    } else if (data.isMember("inline_keyboard")) {
        return parse<InlineKeyboardMarkup>(data);
    }
    return nullptr;
}

DECLARE_PARSER_TO_JSON(GenericReply) {
    if (!object) {
        return {};
    }
    if (std::dynamic_pointer_cast<ForceReply>(object) != nullptr) {
        return put<ForceReply>(object);
    } else if (std::dynamic_pointer_cast<ReplyKeyboardRemove>(object) !=
               nullptr) {
        return put<ReplyKeyboardRemove>(object);
    } else if (std::dynamic_pointer_cast<ReplyKeyboardMarkup>(object) !=
               nullptr) {
        return put<ReplyKeyboardMarkup>(object);
    } else if (std::dynamic_pointer_cast<InlineKeyboardMarkup>(object) !=
               nullptr) {
        return put<InlineKeyboardMarkup>(object);
    }
    return {};
}

}  // namespace TgBot