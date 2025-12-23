#include <nlohmann/json.hpp>
#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>

#include <cstdint>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace TgBot {

// Constant for InaccessibleMessage date field
constexpr std::int64_t INACCESSIBLE_MESSAGE_DATE = 0;

// T should be instance of std::shared_ptr.
template <typename T>
std::shared_ptr<T> parse(const nlohmann::json &data, const std::string &key) {
    if (!data.contains(key)) {
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
    JsonWrapper() : data_(nlohmann::json::object()) {}

    template <typename T>
    void put(const std::string_view key, T value) {
        data_[std::string(key)] = std::move(value);
    }
    void put(const std::string_view key, nlohmann::json value) {
        if (value.empty()) {
            return;
        }
        data_[std::string(key)] = std::move(value);
    }
    template <typename T,
              std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::optional<T> value) {
        if (!value) {
            return;  // Skip empty optional
        }
        data_[std::string(key)] = *value;
    }

    static void merge(nlohmann::json &thiz, const nlohmann::json &other) {
        if (!thiz.is_object() || !other.is_object()) {
            return;
        }

        for (auto it = other.begin(); it != other.end(); ++it) {
            if (thiz[it.key()].is_object()) {
                merge(thiz[it.key()], it.value());
            } else {
                thiz[it.key()] = it.value();
            }
        }
    }

    void operator+=(const nlohmann::json &other) { merge(data_, other); }

    JsonWrapper &operator=(nlohmann::json &&other) {
        data_ = std::forward<decltype(other)>(other);
        return *this;
    }
    operator nlohmann::json() const { return data_; }

   private:
    nlohmann::json data_;
};

template <typename T, std::enable_if_t<detail::is_primitive_v<T> ||
                                           detail::is_optional_v<T>,
                                       bool> = true>
void parse(const nlohmann::json &data, const std::string &key, T *value) {
    using Type = std::conditional_t<detail::is_optional_v<T>,
                                    typename detail::is_optional<T>::type, T>;
    using FixedType =
        std::conditional_t<std::is_floating_point_v<Type>, double, Type>;
    using MoreFixedType =
        std::conditional_t<std::is_integral_v<Type>, int64_t, FixedType>;
    if (data.contains(key) && !data[key].is_null()) {
        *value = static_cast<Type>(data[key].get<MoreFixedType>());
    }
}

// Pre-defined parsing functions for known types.
DECLARE_PARSER_FROM_JSON(Message) {
    auto result = std::make_shared<Message>();
    parse(data, "message_id", &result->messageId);
    parse(data, "message_thread_id", &result->messageThreadId);
    result->from = parse<User>(data, "from");
    result->senderChat = parse<Chat>(data, "sender_chat");
    parse(data, "sender_boost_count", &result->senderBoostCount);
    result->senderBusinessBot = parse<User>(data, "sender_business_bot");
    parse(data, "date", &result->date);
    parse(data, "business_connection_id", &result->businessConnectionId);
    result->chat = parse<Chat>(data, "chat");
    result->forwardOrigin = parse<MessageOrigin>(data, "forward_origin");
    parse(data, "is_topic_message", &result->isTopicMessage);
    parse(data, "is_automatic_forward", &result->isAutomaticForward);
    result->replyToMessage = parse<Message>(data, "reply_to_message");
    result->externalReply = parse<ExternalReplyInfo>(data, "external_reply");
    result->quote = parse<TextQuote>(data, "quote");
    result->replyToStory = parse<Story>(data, "reply_to_story");
    result->viaBot = parse<User>(data, "via_bot");
    parse(data, "edit_date", &result->editDate);
    parse(data, "has_protected_content", &result->hasProtectedContent);
    parse(data, "is_from_offline", &result->isFromOffline);
    parse(data, "media_group_id", &result->mediaGroupId);
    parse(data, "author_signature", &result->authorSignature);
    parse(data, "text", &result->text);
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
    parse(data, "caption", &result->caption);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "has_media_spoiler", &result->hasMediaSpoiler);
    result->contact = parse<Contact>(data, "contact");
    result->dice = parse<Dice>(data, "dice");
    result->game = parse<Game>(data, "game");
    result->poll = parse<Poll>(data, "poll");
    result->venue = parse<Venue>(data, "venue");
    result->location = parse<Location>(data, "location");
    result->newChatMembers = parseArray<User>(data, "new_chat_members");
    result->leftChatMember = parse<User>(data, "left_chat_member");
    parse(data, "new_chat_title", &result->newChatTitle);
    result->newChatPhoto = parseArray<PhotoSize>(data, "new_chat_photo");
    parse(data, "delete_chat_photo", &result->deleteChatPhoto);
    parse(data, "group_chat_created", &result->groupChatCreated);
    parse(data, "supergroup_chat_created", &result->supergroupChatCreated);
    parse(data, "channel_chat_created", &result->channelChatCreated);
    result->messageAutoDeleteTimerChanged =
        parse<MessageAutoDeleteTimerChanged>(
            data, "message_auto_delete_timer_changed");
    parse(data, "migrate_to_chat_id", &result->migrateToChatId);
    parse(data, "migrate_from_chat_id", &result->migrateFromChatId);
    result->pinnedMessage = parse<Message>(data, "pinned_message");
    result->invoice = parse<Invoice>(data, "invoice");
    result->successfulPayment =
        parse<SuccessfulPayment>(data, "successful_payment");
    result->usersShared = parse<UsersShared>(data, "users_shared");
    result->chatShared = parse<ChatShared>(data, "chat_shared");
    parse(data, "connected_website", &result->connectedWebsite);
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
    parse(data, "update_id", &result->updateId);
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
    parse(data, "url", &result->url);
    parse(data, "has_custom_certificate", &result->hasCustomCertificate);
    parse(data, "pending_update_count", &result->pendingUpdateCount);
    parse(data, "ip_address", &result->ipAddress);
    parse(data, "last_error_date", &result->lastErrorDate);
    parse(data, "last_error_message", &result->lastErrorMessage);
    parse(data, "last_synchronization_error_date",
          &result->lastSynchronizationErrorDate);
    parse(data, "max_connections", &result->maxConnections);
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
    parse(data, "id", &result->id);
    parse(data, "is_bot", &result->isBot);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "username", &result->username);
    parse(data, "language_code", &result->languageCode);
    parse(data, "is_premium", &result->isPremium);
    parse(data, "added_to_attachment_menu", &result->addedToAttachmentMenu);
    parse(data, "can_join_groups", &result->canJoinGroups);
    parse(data, "can_read_all_group_messages",
          &result->canReadAllGroupMessages);
    parse(data, "supports_inline_queries", &result->supportsInlineQueries);
    parse(data, "can_connect_to_business", &result->canConnectToBusiness);
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
    parse(data, "id", &result->id);
    std::string type;
    parse(data, "type", &type);
    if (type == "private") {
        result->type = Chat::Type::Private;
    } else if (type == "group") {
        result->type = Chat::Type::Group;
    } else if (type == "supergroup") {
        result->type = Chat::Type::Supergroup;
    } else if (type == "channel") {
        result->type = Chat::Type::Channel;
    }
    parse(data, "title", &result->title);
    parse(data, "username", &result->username);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "is_forum", &result->isForum);
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
    parse(data, "accent_color_id", &result->accentColorId);
    parse(data, "background_custom_emoji_id", &result->backgroundCustomEmojiId);
    parse(data, "profile_accent_color_id", &result->profileAccentColorId);
    parse(data, "profile_background_custom_emoji_id",
          &result->profileBackgroundCustomEmojiId);
    parse(data, "emoji_status_custom_emoji_id",
          &result->emojiStatusCustomEmojiId);
    parse(data, "emoji_status_expiration_date",
          &result->emojiStatusExpirationDate);
    parse(data, "bio", &result->bio);
    parse(data, "has_private_forwards", &result->hasPrivateForwards);
    parse(data, "has_restricted_voice_and_video_messages",
          &result->hasRestrictedVoiceAndVideoMessages);
    parse(data, "join_to_send_messages", &result->joinToSendMessages);
    parse(data, "join_by_request", &result->joinByRequest);
    parse(data, "description", &result->description);
    parse(data, "invite_link", &result->inviteLink);
    result->pinnedMessage = parse<Message>(data, "pinned_message");
    result->permissions = parse<ChatPermissions>(data, "permissions");
    parse(data, "slow_mode_delay", &result->slowModeDelay);
    parse(data, "unrestrict_boost_count", &result->unrestrictBoostCount);
    parse(data, "message_auto_delete_time", &result->messageAutoDeleteTime);
    parse(data, "has_aggressive_anti_spam_enabled",
          &result->hasAggressiveAntiSpamEnabled);
    parse(data, "has_hidden_members", &result->hasHiddenMembers);
    parse(data, "has_protected_content", &result->hasProtectedContent);
    parse(data, "has_visible_history", &result->hasVisibleHistory);
    parse(data, "sticker_set_name", &result->stickerSetName);
    parse(data, "can_set_sticker_set", &result->canSetStickerSet);
    parse(data, "custom_emoji_sticker_set_name",
          &result->customEmojiStickerSetName);
    parse(data, "linked_chat_id", &result->linkedChatId);
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
    parse(data, "message_id", &result->messageId);
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
    parse(data, "message_id", &result->messageId);
    // InaccessibleMessage always has date = 0
    result->date = INACCESSIBLE_MESSAGE_DATE;
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
    std::string type;
    parse(data, "type", &type);
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
    parse(data, "offset", &result->offset);
    parse(data, "length", &result->length);
    parse(data, "url", &result->url);
    result->user = parse<User>(data, "user");
    parse(data, "language", &result->language);
    parse(data, "custom_emoji_id", &result->customEmojiId);
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
    parse(data, "text", &result->text);
    result->entities = parseArray<MessageEntity>(data, "entities");
    parse(data, "position", &result->position);
    parse(data, "is_manual", &result->isManual);
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
    parse(data, "message_id", &result->messageId);
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
    parse(data, "has_media_spoiler", &result->hasMediaSpoiler);
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
    parse(data, "message_id", &result->messageId);
    parse(data, "chat_id", &result->chatId);
    parse(data, "allow_sending_without_reply",
          &result->allowSendingWithoutReply);
    parse(data, "quote", &result->quote);
    parse(data, "quote_parse_mode", &result->quoteParseMode);
    result->quoteEntities = parseArray<MessageEntity>(data, "quote_entities");
    parse(data, "quote_position", &result->quotePosition);
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
    MessageOrigin::Ptr result;
    std::string type;
    parse(data, "type", &type);

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
    parse(data, "date", &result->date);

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
            throw invalidType("MessageOrigin", object->type);
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
    parse(data, "sender_user_name", &result->senderUserName);
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
    parse(data, "author_signature", &result->authorSignature);
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
    parse(data, "message_id", &result->messageId);
    parse(data, "author_signature", &result->authorSignature);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "file_size", &result->fileSize);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "file_name", &result->fileName);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "duration", &result->duration);
    parse(data, "performer", &result->performer);
    parse(data, "title", &result->title);
    parse(data, "file_name", &result->fileName);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "file_name", &result->fileName);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
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
    parse(data, "id", &result->id);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "file_name", &result->fileName);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "length", &result->length);
    parse(data, "duration", &result->duration);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "file_size", &result->fileSize);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "duration", &result->duration);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
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
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "user_id", &result->userId);
    parse(data, "vcard", &result->vcard);
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
    parse(data, "emoji", &result->emoji);
    parse(data, "value", &result->value);
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
    parse(data, "text", &result->text);
    parse(data, "voter_count", &result->voterCount);
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
    parse(data, "poll_id", &result->pollId);
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
    parse(data, "id", &result->id);
    parse(data, "question", &result->question);
    result->options = parseArray<PollOption>(data, "options");
    parse(data, "total_voter_count", &result->totalVoterCount);
    parse(data, "is_closed", &result->isClosed);
    parse(data, "is_anonymous", &result->isAnonymous);
    parse(data, "type", &result->type);
    parse(data, "allows_multiple_answers", &result->allowsMultipleAnswers);
    parse(data, "correct_option_id", &result->correctOptionId);
    parse(data, "explanation", &result->explanation);
    result->explanationEntities =
        parseArray<MessageEntity>(data, "explanation_entities");
    parse(data, "open_period", &result->openPeriod);
    parse(data, "close_date", &result->closeDate);
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
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "horizontal_accuracy", &result->horizontalAccuracy);
    parse(data, "live_period", &result->livePeriod);
    parse(data, "heading", &result->heading);
    parse(data, "proximity_alert_radius", &result->proximityAlertRadius);
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
    parse(data, "title", &result->title);
    parse(data, "address", &result->address);
    parse(data, "foursquare_id", &result->foursquareId);
    parse(data, "foursquare_type", &result->foursquareType);
    parse(data, "google_place_id", &result->googlePlaceId);
    parse(data, "google_place_type", &result->googlePlaceType);
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
    parse(data, "data", &result->data);
    parse(data, "button_text", &result->buttonText);
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
    parse(data, "distance", &result->distance);
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
    parse(data, "message_auto_delete_time", &result->messageAutoDeleteTime);
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
    parse(data, "boost_count", &result->boostCount);
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
    parse(data, "name", &result->name);
    parse(data, "icon_color", &result->iconColor);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
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
    parse(data, "name", &result->name);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
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
    parse(data, "user_id", &result->userId);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "username", &result->username);
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
    parse(data, "request_id", &result->requestId);
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
    parse(data, "request_id", &result->requestId);
    parse(data, "chat_id", &result->chatId);
    parse(data, "title", &result->title);
    parse(data, "username", &result->username);
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
    parse(data, "from_request", &result->fromRequest);
    parse(data, "web_app_name", &result->webAppName);
    parse(data, "from_attachment_menu", &result->fromAttachmentMenu);
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
    parse(data, "start_date", &result->startDate);
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
    parse(data, "duration", &result->duration);
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
    parse(data, "winners_selection_date", &result->winnersSelectionDate);
    parse(data, "winner_count", &result->winnerCount);
    parse(data, "only_new_members", &result->onlyNewMembers);
    parse(data, "has_public_winners", &result->hasPublicWinners);
    parse(data, "prize_description", &result->prizeDescription);
    result->countryCodes =
        parsePrimitiveArray<std::string>(data, "country_codes");
    parse(data, "premium_subscription_month_count",
          &result->premiumSubscriptionMonthCount);
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
    parse(data, "giveaway_message_id", &result->giveawayMessageId);
    parse(data, "winners_selection_date", &result->winnersSelectionDate);
    parse(data, "winner_count", &result->winnerCount);
    result->winners = parseArray<User>(data, "winners");
    parse(data, "additional_chat_count", &result->additionalChatCount);
    parse(data, "premium_subscription_month_count",
          &result->premiumSubscriptionMonthCount);
    parse(data, "unclaimed_prize_count", &result->unclaimedPrizeCount);
    parse(data, "only_new_members", &result->onlyNewMembers);
    parse(data, "was_refunded", &result->wasRefunded);
    parse(data, "prize_description", &result->prizeDescription);
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
    parse(data, "winner_count", &result->winnerCount);
    parse(data, "unclaimed_prize_count", &result->unclaimedPrizeCount);
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
    parse(data, "is_disabled", &result->isDisabled);
    parse(data, "url", &result->url);
    parse(data, "prefer_small_media", &result->preferSmallMedia);
    parse(data, "prefer_large_media", &result->preferLargeMedia);
    parse(data, "show_above_text", &result->showAboveText);
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
    parse(data, "total_count", &result->totalCount);
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
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "file_size", &result->fileSize);
    parse(data, "file_path", &result->filePath);
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
    parse(data, "url", &result->url);
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
    parse(data, "is_persistent", &result->isPersistent);
    parse(data, "resize_keyboard", &result->resizeKeyboard);
    parse(data, "one_time_keyboard", &result->oneTimeKeyboard);
    parse(data, "input_field_placeholder", &result->inputFieldPlaceholder);
    parse(data, "selective", &result->selective);
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
    parse(data, "text", &result->text);
    result->requestUsers =
        parse<KeyboardButtonRequestUsers>(data, "request_users");
    result->requestChat =
        parse<KeyboardButtonRequestChat>(data, "request_chat");
    parse(data, "request_contact", &result->requestContact);
    parse(data, "request_location", &result->requestLocation);
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
    parse(data, "request_id", &result->requestId);
    parse(data, "user_is_bot", &result->userIsBot);
    parse(data, "user_is_premium", &result->userIsPremium);
    parse(data, "max_quantity", &result->maxQuantity);
    parse(data, "request_name", &result->requestName);
    parse(data, "request_username", &result->requestUsername);
    parse(data, "request_photo", &result->requestPhoto);
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
    parse(data, "request_id", &result->requestId);
    parse(data, "chat_is_channel", &result->chatIsChannel);
    parse(data, "chat_is_forum", &result->chatIsForum);
    parse(data, "chat_has_username", &result->chatHasUsername);
    parse(data, "chat_is_created", &result->chatIsCreated);
    result->userAdministratorRights =
        parse<ChatAdministratorRights>(data, "user_administrator_rights");
    result->botAdministratorRights =
        parse<ChatAdministratorRights>(data, "bot_administrator_rights");
    parse(data, "bot_is_member", &result->botIsMember);
    parse(data, "request_title", &result->requestTitle);
    parse(data, "request_username", &result->requestUsername);
    parse(data, "request_photo", &result->requestPhoto);
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonRequestChat) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonPollType) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("type", object->type);
    return json;
}

// Function for parsing ReplyKeyboardRemove from JSON
DECLARE_PARSER_FROM_JSON(ReplyKeyboardRemove) {
    auto result = std::make_shared<ReplyKeyboardRemove>();
    parse(data, "remove_keyboard", &result->removeKeyboard);
    parse(data, "selective", &result->selective);
    return result;
}

DECLARE_PARSER_TO_JSON(ReplyKeyboardRemove) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("inline_keyboard", put(object->inlineKeyboard));
    return json;
}
DECLARE_PARSER_FROM_JSON(ForceReply) {
    auto result = std::make_shared<ForceReply>();
    parse(data, "force_reply", &result->forceReply);
    parse(data, "input_field_placeholder", &result->inputFieldPlaceholder);
    parse(data, "selective", &result->selective);
    return result;
}

DECLARE_PARSER_TO_JSON(ForceReply) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("force_reply", object->forceReply);
    json.put("input_field_placeholder", object->inputFieldPlaceholder);
    json.put("selective", object->selective);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatPhoto) {
    auto result = std::make_shared<ChatPhoto>();
    parse(data, "small_file_id", &result->smallFileId);
    parse(data, "small_file_unique_id", &result->smallFileUniqueId);
    parse(data, "big_file_id", &result->bigFileId);
    parse(data, "big_file_unique_id", &result->bigFileUniqueId);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatPhoto) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("small_file_id", object->smallFileId);
    json.put("small_file_unique_id", object->smallFileUniqueId);
    json.put("big_file_id", object->bigFileId);
    json.put("big_file_unique_id", object->bigFileUniqueId);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatInviteLink) {
    auto result = std::make_shared<ChatInviteLink>();
    parse(data, "invite_link", &result->inviteLink);
    result->creator = parse<User>(data, "creator");
    parse(data, "creates_join_request", &result->createsJoinRequest);
    parse(data, "is_primary", &result->isPrimary);
    parse(data, "is_revoked", &result->isRevoked);
    parse(data, "name", &result->name);
    parse(data, "expire_date", &result->expireDate);
    parse(data, "member_limit", &result->memberLimit);
    parse(data, "pending_join_request_count", &result->pendingJoinRequestCount);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatInviteLink) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    return result;
}

DECLARE_PARSER_TO_JSON(ChatAdministratorRights) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    parse(data, "date", &result->date);
    result->oldChatMember = parse<ChatMember>(data, "old_chat_member");
    result->newChatMember = parse<ChatMember>(data, "new_chat_member");
    result->inviteLink = parse<ChatInviteLink>(data, "invite_link");
    parse(data, "via_chat_folder_invite_link",
          &result->viaChatFolderInviteLink);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberUpdated) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    std::string status;
    parse(data, "status", &status);
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
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    parse(data, "is_anonymous", &result->isAnonymous);
    parse(data, "custom_title", &result->customTitle);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberOwner) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("is_anonymous", object->isAnonymous);
    json.put("custom_title", object->customTitle);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatMemberAdministrator) {
    auto result = std::make_shared<ChatMemberAdministrator>();
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
    parse(data, "custom_title", &result->customTitle);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberAdministrator) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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

DECLARE_PARSER_TO_JSON(ChatMemberMember) { return JsonWrapper{}; }

DECLARE_PARSER_FROM_JSON(ChatMemberRestricted) {
    auto result = std::make_shared<ChatMemberRestricted>();
    parse(data, "is_member", &result->isMember);
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
    parse(data, "can_change_info", &result->canChangeInfo);
    parse(data, "can_invite_users", &result->canInviteUsers);
    parse(data, "can_pin_messages", &result->canPinMessages);
    parse(data, "can_manage_topics", &result->canManageTopics);
    parse(data, "until_date", &result->untilDate);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberRestricted) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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

DECLARE_PARSER_TO_JSON(ChatMemberLeft) { return JsonWrapper{}; }

DECLARE_PARSER_FROM_JSON(ChatMemberBanned) {
    auto result = std::make_shared<ChatMemberBanned>();
    parse(data, "until_date", &result->untilDate);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberBanned) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("until_date", object->untilDate);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatJoinRequest) {
    auto result = std::make_shared<ChatJoinRequest>();
    result->chat = parse<Chat>(data, "chat");
    result->from = parse<User>(data, "from");
    parse(data, "user_chat_id", &result->userChatId);
    parse(data, "date", &result->date);
    parse(data, "bio", &result->bio);
    result->inviteLink = parse<ChatInviteLink>(data, "invite_link");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatJoinRequest) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    parse(data, "can_change_info", &result->canChangeInfo);
    parse(data, "can_invite_users", &result->canInviteUsers);
    parse(data, "can_pin_messages", &result->canPinMessages);
    parse(data, "can_manage_topics", &result->canManageTopics);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatPermissions) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    parse(data, "day", &result->day);
    parse(data, "month", &result->month);
    parse(data, "year", &result->year);
    return result;
}

DECLARE_PARSER_TO_JSON(Birthdate) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("day", object->day);
    json.put("month", object->month);
    json.put("year", object->year);
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessIntro) {
    auto result = std::make_shared<BusinessIntro>();
    parse(data, "title", &result->title);
    parse(data, "message", &result->message);
    result->sticker = parse<Sticker>(data, "sticker");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessIntro) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("title", object->title);
    json.put("message", object->message);
    json.put("sticker", put(object->sticker));
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessLocation) {
    auto result = std::make_shared<BusinessLocation>();
    parse(data, "address", &result->address);
    result->location = parse<Location>(data, "location");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessLocation) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("address", object->address);
    json.put("location", put(object->location));
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessOpeningHoursInterval) {
    auto result = std::make_shared<BusinessOpeningHoursInterval>();
    parse(data, "opening_minute", &result->openingMinute);
    parse(data, "closing_minute", &result->closingMinute);
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessOpeningHoursInterval) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("opening_minute", object->openingMinute);
    json.put("closing_minute", object->closingMinute);
    return json;
}

DECLARE_PARSER_FROM_JSON(BusinessOpeningHours) {
    auto result = std::make_shared<BusinessOpeningHours>();
    parse(data, "time_zone_name", &result->timeZoneName);
    result->openingHours =
        parseArray<BusinessOpeningHoursInterval>(data, "opening_hours");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessOpeningHours) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("time_zone_name", object->timeZoneName);
    json.put("opening_hours", put(object->openingHours));
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatLocation) {
    auto result = std::make_shared<ChatLocation>();
    result->location = parse<Location>(data, "location");
    parse(data, "address", &result->address);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatLocation) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("location", put(object->location));
    json.put("address", object->address);
    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionType) {
    ReactionType::Ptr result;
    std::string type;

    parse(data, "type", &type);
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
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    parse(data, "emoji", &result->emoji);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionTypeEmoji) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("emoji", object->emoji);
    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionTypeCustomEmoji) {
    auto result = std::make_shared<ReactionTypeCustomEmoji>();
    parse(data, "custom_emoji_id", &result->customEmojiId);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionTypeCustomEmoji) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("custom_emoji_id", object->customEmojiId);
    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionCount) {
    auto result = std::make_shared<ReactionCount>();
    result->type = parse<ReactionType>(data, "type");
    parse(data, "total_count", &result->totalCount);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionCount) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("type", put(object->type));
    json.put("total_count", object->totalCount);
    return json;
}

DECLARE_PARSER_FROM_JSON(MessageReactionUpdated) {
    auto result = std::make_shared<MessageReactionUpdated>();
    result->chat = parse<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    result->user = parse<User>(data, "user");
    result->actorChat = parse<Chat>(data, "actor_chat");
    parse(data, "date", &result->date);
    result->oldReaction = parseArray<ReactionType>(data, "old_reaction");
    result->newReaction = parseArray<ReactionType>(data, "new_reaction");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionUpdated) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
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
    parse(data, "message_id", &result->messageId);
    parse(data, "date", &result->date);
    result->reactions = parseArray<ReactionCount>(data, "reactions");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionCountUpdated) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("chat", put(object->chat));
    json.put("message_id", object->messageId);
    json.put("date", object->date);
    json.put("reactions", put(object->reactions));
    return json;
}

DECLARE_PARSER_FROM_JSON(ForumTopic) {
    auto result = std::make_shared<ForumTopic>();
    parse(data, "message_thread_id", &result->messageThreadId);
    parse(data, "name", &result->name);
    parse(data, "icon_color", &result->iconColor);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
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
    parse(data, "command", &result->command);
    parse(data, "description", &result->description);
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
    std::string type;
    BotCommandScope::Ptr result;

    parse(data, "type", &type);
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
    if (!object) return ptree;

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
    parse(data, "chat_id", &result->chatId);
    return result;
}

// Serializing BotCommandScopeChatAdministrators to JSON
DECLARE_PARSER_TO_JSON(BotCommandScopeChatAdministrators) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("chat_id", object->chatId);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(BotCommandScopeChat) {
    auto result = std::make_shared<BotCommandScopeChat>();
    parse(data, "chat_id", &result->chatId);
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
    parse(data, "chat_id", &result->chatId);
    parse(data, "user_id", &result->userId);
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
    parse(data, "name", &result->name);
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
    parse(data, "description", &result->description);
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
    parse(data, "short_description", &result->shortDescription);
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
    std::string source;
    ChatBoostSource::Ptr result;

    parse(data, "source", &source);
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
    std::string type;
    MenuButton::Ptr result;

    parse(data, "type", &type);
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
    parse(data, "boost_id", &result->boostId);
    parse(data, "add_date", &result->addDate);
    parse(data, "expiration_date", &result->expirationDate);
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
    parse(data, "boost_id", &result->boostId);
    parse(data, "remove_date", &result->removeDate);
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
    parse(data, "text", &result->text);
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
    parse(data, "giveaway_message_id", &result->giveawayMessageId);
    parse(data, "is_unclaimed", &result->isUnclaimed);
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
    parse(data, "id", &result->id);
    result->user = parse<User>(data, "user");
    parse(data, "user_chat_id", &result->userChatId);
    parse(data, "date", &result->date);
    parse(data, "can_reply", &result->canReply);
    parse(data, "is_enabled", &result->isEnabled);
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
    parse(data, "business_connection_id", &result->businessConnectionId);
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
    parse(data, "migrate_to_chat_id", &result->migrateToChatId);
    parse(data, "retry_after", &result->retryAfter);
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
    InputMedia::Ptr result;
    std::string type;

    parse(data, "type", &type);
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

    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
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
    parse(data, "has_spoiler", &result->hasSpoiler);
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
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    parse(data, "supports_streaming", &result->supportsStreaming);
    parse(data, "has_spoiler", &result->hasSpoiler);
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
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    parse(data, "has_spoiler", &result->hasSpoiler);
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
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "duration", &result->duration);
    parse(data, "performer", &result->performer);
    parse(data, "title", &result->title);
    return result;
}

// InputMediaDocument
DECLARE_PARSER_FROM_JSON(InputMediaDocument) {
    auto result = std::make_shared<InputMediaDocument>();
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "disable_content_type_detection",
          &result->disableContentTypeDetection);
    return result;
}

DECLARE_PARSER_TO_JSON(InputMediaDocument) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("thumbnail", object->thumbnail);
    ptree.put("disable_content_type_detection",
              object->disableContentTypeDetection);
    return ptree;
}

// Sticker
DECLARE_PARSER_FROM_JSON(Sticker) {
    auto result = std::make_shared<Sticker>();
    std::string type;
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "type", &type);
    if (type == "regular") {
        result->type = Sticker::Type::Regular;
    } else if (type == "mask") {
        result->type = Sticker::Type::Mask;
    } else if (type == "custom_emoji") {
        result->type = Sticker::Type::CustomEmoji;
    }
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "is_animated", &result->isAnimated);
    parse(data, "is_video", &result->isVideo);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "emoji", &result->emoji);
    parse(data, "set_name", &result->setName);
    result->premiumAnimation = parse<File>(data, "premium_animation");
    result->maskPosition = parse<MaskPosition>(data, "mask_position");
    parse(data, "custom_emoji_id", &result->customEmojiId);
    parse(data, "needs_repainting", &result->needsRepainting);
    parse(data, "file_size", &result->fileSize);
    return result;
}

DECLARE_PARSER_TO_JSON(Sticker) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    std::string stickerType;
    parse(data, "name", &result->name);
    parse(data, "title", &result->title);
    parse(data, "sticker_type", &stickerType);
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

DECLARE_PARSER_TO_JSON(BotCommandScopeDefault) { return JsonWrapper{}; }

// BotCommandScopeAllPrivateChats
DECLARE_PARSER_FROM_JSON(BotCommandScopeAllPrivateChats) {
    return std::make_shared<BotCommandScopeAllPrivateChats>();
}

DECLARE_PARSER_TO_JSON(BotCommandScopeAllPrivateChats) { return JsonWrapper{}; }

// BotCommandScopeAllGroupChats
DECLARE_PARSER_FROM_JSON(BotCommandScopeAllGroupChats) {
    return std::make_shared<BotCommandScopeAllGroupChats>();
}

DECLARE_PARSER_TO_JSON(BotCommandScopeAllGroupChats) { return JsonWrapper{}; }

// BotCommandScopeAllChatAdministrators
DECLARE_PARSER_FROM_JSON(BotCommandScopeAllChatAdministrators) {
    return std::make_shared<BotCommandScopeAllChatAdministrators>();
}

DECLARE_PARSER_TO_JSON(BotCommandScopeAllChatAdministrators) {
    return JsonWrapper{};
}

DECLARE_PARSER_TO_JSON(StickerSet) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "id", &result->id);
    result->from = parse<User>(data, "from");
    // Parse MaybeInaccessibleMessage: check if message exists and determine type
    if (data.isMember("message") && !data["message"].isNull()) {
        const auto& messageData = data["message"];
        // According to Bot API, InaccessibleMessage has date=0, Message has date>0
        if (messageData.isMember("date") && messageData["date"].asInt64() == INACCESSIBLE_MESSAGE_DATE) {
            result->message = parse<InaccessibleMessage>(messageData);
        } else {
            result->message = parse<Message>(messageData);
        }
    }
    parse(data, "inline_message_id", &result->inlineMessageId);
    parse(data, "chat_instance", &result->chatInstance);
    parse(data, "data", &result->data);
    parse(data, "game_short_name", &result->gameShortName);
    return result;
}

DECLARE_PARSER_TO_JSON(CallbackQuery) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("id", object->id);
    ptree.put("from", put(object->from));
    // Handle MaybeInaccessibleMessage variant using std::visit
    if (object->message.has_value()) {
        std::visit([&ptree](auto&& msg) {
            ptree.put("message", put(msg));
        }, object->message.value());
    }
    ptree.put("inline_message_id", object->inlineMessageId);
    ptree.put("chat_instance", object->chatInstance);
    ptree.put("data", object->data);
    ptree.put("game_short_name", object->gameShortName);
    return ptree;
}

// MaskPosition
DECLARE_PARSER_FROM_JSON(MaskPosition) {
    auto result = std::make_shared<MaskPosition>();
    parse(data, "point", &result->point);
    parse(data, "x_shift", &result->xShift);
    parse(data, "y_shift", &result->yShift);
    parse(data, "scale", &result->scale);
    return result;
}

DECLARE_PARSER_TO_JSON(MaskPosition) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("point", object->point);
    ptree.put("x_shift", object->xShift);
    ptree.put("y_shift", object->yShift);
    ptree.put("scale", object->scale);
    return ptree;
}
// Parsing from JSON to InlineQuery
DECLARE_PARSER_FROM_JSON(InlineQuery) {
    auto result = std::make_shared<InlineQuery>();
    parse(data, "id", &result->id);
    result->from = parse<User>(data, "from");
    parse(data, "query", &result->query);
    parse(data, "offset", &result->offset);
    parse(data, "chat_type", &result->chatType);
    result->location = parse<Location>(data, "location");
    return result;
}

// Serializing InlineQuery to JSON
DECLARE_PARSER_TO_JSON(InlineQuery) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "text", &result->text);
    parse(data, "url", &result->url);
    parse(data, "callback_data", &result->callbackData);
    result->webApp = parse<WebAppInfo>(data, "web_app");
    result->loginUrl = parse<LoginUrl>(data, "login_url");
    parse(data, "switch_inline_query", &result->switchInlineQuery);
    parse(data, "switch_inline_query_current_chat",
          &result->switchInlineQueryCurrentChat);
    result->switchInlineQueryChosenChat = parse<SwitchInlineQueryChosenChat>(
        data, "switch_inline_query_chosen_chat");
    result->callbackGame = parse<CallbackGame>(data, "callback_game");
    parse(data, "pay", &result->pay);
    return result;
}

// Serializing InlineKeyboardButton to JSON
DECLARE_PARSER_TO_JSON(InlineKeyboardButton) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    if (ptr->isCached) {
        return put<CachedT>(ptr);
    } else {
        return put<T>(ptr);
    }
}

DECLARE_PARSER_TO_JSON(InlineQueryResult) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "sticker", &result->sticker);
    parse(data, "format", &result->format);
    result->emojiList = parsePrimitiveArray<std::string>(data, "emoji_list");
    result->maskPosition = parse<MaskPosition>(data, "mask_position");
    result->keywords = parsePrimitiveArray<std::string>(data, "keywords");
    return result;
}

// Serializing InputSticker to JSON
DECLARE_PARSER_TO_JSON(InputSticker) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "query", &result->query);
    parse(data, "allow_user_chats", &result->allowUserChats);
    parse(data, "allow_bot_chats", &result->allowBotChats);
    parse(data, "allow_group_chats", &result->allowGroupChats);
    parse(data, "allow_channel_chats", &result->allowChannelChats);
    return result;
}

// Serializing SwitchInlineQueryChosenChat to JSON
DECLARE_PARSER_TO_JSON(SwitchInlineQueryChosenChat) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "url", &result->url);
    parse(data, "forward_text", &result->forwardText);
    parse(data, "bot_username", &result->botUsername);
    parse(data, "request_write_access", &result->requestWriteAccess);
    return result;
}

// Serializing LoginUrl to JSON
DECLARE_PARSER_TO_JSON(LoginUrl) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("url", object->url);
    ptree.put("forward_text", object->forwardText);
    ptree.put("bot_username", object->botUsername);
    ptree.put("request_write_access", object->requestWriteAccess);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultsButton) {
    auto result = std::make_shared<InlineQueryResultsButton>();
    parse(data, "text", &result->text);
    result->webApp = parse<WebAppInfo>(data, "web_app");
    parse(data, "start_parameter", &result->startParameter);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultsButton) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("text", object->text);
    ptree.put("web_app", put(object->webApp));
    ptree.put("start_parameter", object->startParameter);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultArticle) {
    auto result = std::make_shared<InlineQueryResultArticle>();
    parse(data, "title", &result->title);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    parse(data, "url", &result->url);
    parse(data, "hide_url", &result->hideUrl);
    parse(data, "description", &result->description);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultArticle) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "photo_url", &result->photoUrl);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "photo_width", &result->photoWidth);
    parse(data, "photo_height", &result->photoHeight);
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultPhoto) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "gif_url", &result->gifUrl);
    parse(data, "gif_width", &result->gifWidth);
    parse(data, "gif_height", &result->gifHeight);
    parse(data, "gif_duration", &result->gifDuration);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_mime_type", &result->thumbnailMimeType);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultGif) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "mpeg4_url", &result->mpeg4Url);
    parse(data, "mpeg4_width", &result->mpeg4Width);
    parse(data, "mpeg4_height", &result->mpeg4Height);
    parse(data, "mpeg4_duration", &result->mpeg4Duration);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_mime_type", &result->thumbnailMimeType);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultMpeg4Gif) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "video_url", &result->videoUrl);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "video_width", &result->videoWidth);
    parse(data, "video_height", &result->videoHeight);
    parse(data, "video_duration", &result->videoDuration);
    parse(data, "description", &result->description);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVideo) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "audio_url", &result->audioUrl);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "performer", &result->performer);
    parse(data, "audio_duration", &result->audioDuration);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultAudio) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "voice_url", &result->voiceUrl);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "voice_duration", &result->voiceDuration);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVoice) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "document_url", &result->documentUrl);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "description", &result->description);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultDocument) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "title", &result->title);
    parse(data, "horizontal_accuracy", &result->horizontalAccuracy);
    parse(data, "live_period", &result->livePeriod);
    parse(data, "heading", &result->heading);
    parse(data, "proximity_alert_radius", &result->proximityAlertRadius);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultLocation) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "title", &result->title);
    parse(data, "address", &result->address);
    parse(data, "foursquare_id", &result->foursquareId);
    parse(data, "foursquare_type", &result->foursquareType);
    parse(data, "google_place_id", &result->googlePlaceId);
    parse(data, "google_place_type", &result->googlePlaceType);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVenue) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "vcard", &result->vcard);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultContact) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "game_short_name", &result->gameShortName);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultGame) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("game_short_name", object->gameShortName);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedPhoto) {
    auto result = std::make_shared<InlineQueryResultCachedPhoto>();
    parse(data, "photo_file_id", &result->photoFileId);
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedPhoto) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "gif_file_id", &result->gifFileId);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedGif) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "mpeg4_file_id", &result->mpeg4FileId);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedMpeg4Gif) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "sticker_file_id", &result->stickerFileId);
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedSticker) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("sticker_file_id", object->stickerFileId);
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedDocument) {
    auto result = std::make_shared<InlineQueryResultCachedDocument>();
    parse(data, "title", &result->title);
    parse(data, "document_file_id", &result->documentFileId);
    parse(data, "description", &result->description);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedDocument) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "video_file_id", &result->videoFileId);
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedVideo) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "voice_file_id", &result->voiceFileId);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedVoice) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "audio_file_id", &result->audioFileId);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities =
        parseArray<MessageEntity>(data, "caption_entities");
    result->inputMessageContent =
        parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedAudio) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("audio_file_id", object->audioFileId);
    ptree.put("caption", object->caption);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("caption_entities", put(object->captionEntities));
    ptree.put("input_message_content", put(object->inputMessageContent));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMessageContent) {
    InputMessageContent::Ptr result;

    std::string messageText;
    float latitude = 1000.F;
    std::string address;
    std::string phoneNumber;
    std::string description;

    parse(data, "message_text", &messageText);
    parse(data, "latitude", &latitude);
    parse(data, "address", &address);
    parse(data, "phone_number", &phoneNumber);
    parse(data, "description", &description);

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
        throw invalidType("InputMessageContent", "");
    }

    return result;
}

DECLARE_PARSER_TO_JSON(InputMessageContent) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }

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
    parse(data, "message_text", &result->messageText);
    parse(data, "parse_mode", &result->parseMode);
    result->entities = parseArray<MessageEntity>(data, "entities");
    result->linkPreviewOptions =
        parse<LinkPreviewOptions>(data, "link_preview_options");
    return result;
}

DECLARE_PARSER_TO_JSON(InputTextMessageContent) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("message_text", object->messageText);
    ptree.put("parse_mode", object->parseMode);
    ptree.put("entities", put(object->entities));
    ptree.put("link_preview_options", put(object->linkPreviewOptions));
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputLocationMessageContent) {
    auto result = std::make_shared<InputLocationMessageContent>();
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "horizontal_accuracy", &result->horizontalAccuracy);
    parse(data, "live_period", &result->livePeriod);
    parse(data, "heading", &result->heading);
    parse(data, "proximity_alert_radius", &result->proximityAlertRadius);
    return result;
}

DECLARE_PARSER_TO_JSON(InputLocationMessageContent) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "title", &result->title);
    parse(data, "address", &result->address);
    parse(data, "foursquare_id", &result->foursquareId);
    parse(data, "foursquare_type", &result->foursquareType);
    parse(data, "google_place_id", &result->googlePlaceId);
    parse(data, "google_place_type", &result->googlePlaceType);
    return result;
}

DECLARE_PARSER_TO_JSON(InputVenueMessageContent) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "vcard", &result->vcard);
    return result;
}

DECLARE_PARSER_TO_JSON(InputContactMessageContent) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("phone_number", object->phoneNumber);
    ptree.put("first_name", object->firstName);
    ptree.put("last_name", object->lastName);
    ptree.put("vcard", object->vcard);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputInvoiceMessageContent) {
    auto result = std::make_shared<InputInvoiceMessageContent>();
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "payload", &result->payload);
    parse(data, "provider_token", &result->providerToken);
    parse(data, "currency", &result->currency);
    result->prices = parseArray<LabeledPrice>(data, "prices");
    parse(data, "max_tip_amount", &result->maxTipAmount);
    result->suggestedTipAmounts =
        parsePrimitiveArray<std::int32_t>(data, "suggested_tip_amounts");
    parse(data, "provider_data", &result->providerData);
    parse(data, "photo_url", &result->photoUrl);
    parse(data, "photo_size", &result->photoSize);
    parse(data, "photo_width", &result->photoWidth);
    parse(data, "photo_height", &result->photoHeight);
    parse(data, "need_name", &result->needName);
    parse(data, "need_phone_number", &result->needPhoneNumber);
    parse(data, "need_email", &result->needEmail);
    parse(data, "need_shipping_address", &result->needShippingAddress);
    parse(data, "send_phone_number_to_provider",
          &result->sendPhoneNumberToProvider);
    parse(data, "send_email_to_provider", &result->sendEmailToProvider);
    parse(data, "is_flexible", &result->isFlexible);
    return result;
}

DECLARE_PARSER_TO_JSON(InputInvoiceMessageContent) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "result_id", &result->resultId);
    result->from = parse<User>(data, "from");
    result->location = parse<Location>(data, "location");
    parse(data, "inline_message_id", &result->inlineMessageId);
    parse(data, "query", &result->query);
    return result;
}

DECLARE_PARSER_TO_JSON(ChosenInlineResult) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("result_id", object->resultId);
    ptree.put("from", put(object->from));
    ptree.put("location", put(object->location));
    ptree.put("inline_message_id", object->inlineMessageId);
    ptree.put("query", object->query);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(SentWebAppMessage) {
    auto result = std::make_shared<SentWebAppMessage>();
    parse(data, "inline_message_id", &result->inlineMessageId);
    return result;
}

DECLARE_PARSER_TO_JSON(SentWebAppMessage) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("inline_message_id", object->inlineMessageId);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(LabeledPrice) {
    auto result = std::make_shared<LabeledPrice>();
    parse(data, "label", &result->label);
    parse(data, "amount", &result->amount);
    return result;
}

DECLARE_PARSER_TO_JSON(LabeledPrice) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("label", object->label);
    ptree.put("amount", object->amount);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(Invoice) {
    auto result = std::make_shared<Invoice>();
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "start_parameter", &result->startParameter);
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    return result;
}

DECLARE_PARSER_TO_JSON(Invoice) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("title", object->title);
    ptree.put("description", object->description);
    ptree.put("start_parameter", object->startParameter);
    ptree.put("currency", object->currency);
    ptree.put("total_amount", object->totalAmount);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ShippingAddress) {
    auto result = std::make_shared<ShippingAddress>();
    parse(data, "country_code", &result->countryCode);
    parse(data, "state", &result->state);
    parse(data, "city", &result->city);
    parse(data, "street_line1", &result->streetLine1);
    parse(data, "street_line2", &result->streetLine2);
    parse(data, "post_code", &result->postCode);
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingAddress) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "name", &result->name);
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "email", &result->email);
    result->shippingAddress = parse<ShippingAddress>(data, "shipping_address");
    return result;
}

DECLARE_PARSER_TO_JSON(OrderInfo) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("name", object->name);
    ptree.put("phone_number", object->phoneNumber);
    ptree.put("email", object->email);
    ptree.put("shipping_address", put(object->shippingAddress));
    return ptree;
}

// ShippingOption
DECLARE_PARSER_FROM_JSON(ShippingOption) {
    auto result = std::make_shared<ShippingOption>();
    parse(data, "id", &result->id);
    parse(data, "title", &result->title);
    result->prices = parseArray<LabeledPrice>(data, "prices");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingOption) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("id", object->id);
    ptree.put("title", object->title);
    ptree.put("prices", put(object->prices));
    return ptree;
}

// SuccessfulPayment
DECLARE_PARSER_FROM_JSON(SuccessfulPayment) {
    auto result = std::make_shared<SuccessfulPayment>();
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "shipping_option_id", &result->shippingOptionId);
    result->orderInfo = parse<OrderInfo>(data, "order_info");
    parse(data, "telegram_payment_charge_id", &result->telegramPaymentChargeId);
    parse(data, "provider_payment_charge_id", &result->providerPaymentChargeId);
    return result;
}

DECLARE_PARSER_TO_JSON(SuccessfulPayment) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "id", &result->id);
    result->from = parse<User>(data, "from");
    parse(data, "invoice_payload", &result->invoicePayload);
    result->shippingAddress = parse<ShippingAddress>(data, "shipping_address");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingQuery) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("id", object->id);
    ptree.put("from", put(object->from));
    ptree.put("invoice_payload", object->invoicePayload);
    ptree.put("shipping_address", put(object->shippingAddress));
    return ptree;
}

// PreCheckoutQuery
DECLARE_PARSER_FROM_JSON(PreCheckoutQuery) {
    auto result = std::make_shared<PreCheckoutQuery>();
    parse(data, "id", &result->id);
    result->from = parse<User>(data, "from");
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "shipping_option_id", &result->shippingOptionId);
    result->orderInfo = parse<OrderInfo>(data, "order_info");
    return result;
}

DECLARE_PARSER_TO_JSON(PreCheckoutQuery) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("data", put(object->data));
    ptree.put("credentials", put(object->credentials));
    return ptree;
}

// PassportFile
DECLARE_PARSER_FROM_JSON(PassportFile) {
    auto result = std::make_shared<PassportFile>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "file_size", &result->fileSize);
    parse(data, "file_date", &result->fileDate);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportFile) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_id", object->fileId);
    ptree.put("file_unique_id", object->fileUniqueId);
    ptree.put("file_size", object->fileSize);
    ptree.put("file_date", object->fileDate);
    return ptree;
}

// EncryptedPassportElement
DECLARE_PARSER_FROM_JSON(EncryptedPassportElement) {
    auto result = std::make_shared<EncryptedPassportElement>();
    parse(data, "type", &result->type);
    parse(data, "data", &result->data);
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "email", &result->email);
    result->files = parseArray<PassportFile>(data, "files");
    result->frontSide = parse<PassportFile>(data, "front_side");
    result->reverseSide = parse<PassportFile>(data, "reverse_side");
    result->selfie = parse<PassportFile>(data, "selfie");
    result->translation = parseArray<PassportFile>(data, "translation");
    parse(data, "hash", &result->hash);
    return result;
}

DECLARE_PARSER_TO_JSON(EncryptedPassportElement) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    parse(data, "data", &result->data);
    parse(data, "hash", &result->hash);
    parse(data, "secret", &result->secret);
    return result;
}

DECLARE_PARSER_TO_JSON(EncryptedCredentials) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("data", object->data);
    ptree.put("hash", object->hash);
    ptree.put("secret", object->secret);
    return ptree;
}

// PassportElementError
DECLARE_PARSER_FROM_JSON(PassportElementError) {
    PassportElementError::Ptr result;
    std::string source;

    parse(data, "source", &source);
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
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    parse(data, "message", &result->message);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementError) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("source", object->source);
    ptree.put("type", object->type);
    ptree.put("message", object->message);
    return ptree;
}

// PassportElementErrorDataField
DECLARE_PARSER_FROM_JSON(PassportElementErrorDataField) {
    auto result = std::make_shared<PassportElementErrorDataField>();
    parse(data, "field_name", &result->fieldName);
    parse(data, "data_hash", &result->dataHash);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorDataField) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("field_name", object->fieldName);
    ptree.put("data_hash", object->dataHash);
    return ptree;
}

// PassportElementErrorFrontSide
DECLARE_PARSER_FROM_JSON(PassportElementErrorFrontSide) {
    auto result = std::make_shared<PassportElementErrorFrontSide>();
    parse(data, "file_hash", &result->fileHash);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorFrontSide) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorReverseSide
DECLARE_PARSER_FROM_JSON(PassportElementErrorReverseSide) {
    auto result = std::make_shared<PassportElementErrorReverseSide>();
    parse(data, "file_hash", &result->fileHash);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorReverseSide) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorSelfie
DECLARE_PARSER_FROM_JSON(PassportElementErrorSelfie) {
    auto result = std::make_shared<PassportElementErrorSelfie>();
    parse(data, "file_hash", &result->fileHash);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorSelfie) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_hash", object->fileHash);
    return ptree;
}

// PassportElementErrorFile
DECLARE_PARSER_FROM_JSON(PassportElementErrorFile) {
    auto result = std::make_shared<PassportElementErrorFile>();
    parse(data, "file_hash", &result->fileHash);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorFile) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_hashes", put(object->fileHashes));
    return ptree;
}

// PassportElementErrorTranslationFile
DECLARE_PARSER_FROM_JSON(PassportElementErrorTranslationFile) {
    auto result = std::make_shared<PassportElementErrorTranslationFile>();
    parse(data, "file_hash", &result->fileHash);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorTranslationFile) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_hashes", put(object->fileHashes));
    return ptree;
}

// PassportElementErrorUnspecified
DECLARE_PARSER_FROM_JSON(PassportElementErrorUnspecified) {
    auto result = std::make_shared<PassportElementErrorUnspecified>();
    parse(data, "element_hash", &result->elementHash);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorUnspecified) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("element_hash", object->elementHash);
    return ptree;
}

// Game
DECLARE_PARSER_FROM_JSON(Game) {
    auto result = std::make_shared<Game>();
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    result->photo = parseArray<PhotoSize>(data, "photo");
    parse(data, "text", &result->text);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    result->animation = parse<Animation>(data, "animation");
    return result;
}

DECLARE_PARSER_TO_JSON(Game) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
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
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    return ptree;
}

// GameHighScore
DECLARE_PARSER_FROM_JSON(GameHighScore) {
    auto result = std::make_shared<GameHighScore>();
    parse(data, "position", &result->position);
    result->user = parse<User>(data, "user");
    parse(data, "score", &result->score);
    return result;
}

DECLARE_PARSER_TO_JSON(GameHighScore) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("position", object->position);
    ptree.put("user", put(object->user));
    ptree.put("score", object->score);
    return ptree;
}

// GenericReply
DECLARE_PARSER_FROM_JSON(GenericReply) {
    if (data.contains("force_reply")) {
        return parse<ForceReply>(data);
    } else if (data.contains("remove_keyboard")) {
        return parse<ReplyKeyboardRemove>(data);
    } else if (data.contains("keyboard")) {
        return parse<ReplyKeyboardMarkup>(data);
    } else if (data.contains("inline_keyboard")) {
        return parse<InlineKeyboardMarkup>(data);
    }
    return nullptr;
}

DECLARE_PARSER_TO_JSON(GenericReply) {
    if (!object) {
        return JsonWrapper{};
    }
    if (ForceReply::TYPE == object->type) {
        return put<ForceReply>(object);
    } else if (ReplyKeyboardRemove::TYPE == object->type) {
        return put<ReplyKeyboardRemove>(object);
    } else if (ReplyKeyboardMarkup::TYPE == object->type) {
        return put<ReplyKeyboardMarkup>(object);
    } else if (InlineKeyboardMarkup::TYPE == object->type) {
        return put<InlineKeyboardMarkup>(object);
    }
    return JsonWrapper{};
}

}  // namespace TgBot
