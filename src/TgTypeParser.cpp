#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>

#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>

namespace TgBot {

// T should be instance of std::shared_ptr.
template <typename T>
std::optional<std::shared_ptr<T>> parse(const nlohmann::json& data, const std::string& key) {
    if (!data.contains(key)) {
        return std::nullopt;
    }
    return parse<T>(data[key]);
}

template <typename T>
std::shared_ptr<T> parseRequired(const nlohmann::json& data, const std::string& key) {
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

    // Overload for primitive types
    template <typename T,
              std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, T value) {
        data_[std::string(key)] = std::move(value);
    }
    // Required objects
    template <typename T,
        std::enable_if_t<!detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::shared_ptr<T> value) {
        data_[std::string(key)] = TgBot::put(value);
    }

    // Support for vector of primitives and objects
    template <typename T>
    void put(const std::string_view key, std::vector<T> value) {
        data_[std::string(key)] = TgBot::put(value);
    }

    // Overload for optional types
    template <typename T,
              std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::optional<T> value) {
        if (!value) {
            return;  // Skip empty optional
        }
        data_[std::string(key)] = *value;
    }
    template <typename T,
        std::enable_if_t<!detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::optional<T> value) {
        if (!value) {
            return;  // Skip empty optional
        }
        data_[std::string(key)] = TgBot::put(*value);
    }

    static void merge(nlohmann::json& thiz, const nlohmann::json& other) {
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

    void operator+=(const nlohmann::json& other) { merge(data_, other); }

    JsonWrapper& operator=(nlohmann::json&& other) {
        data_ = std::move(other);
        return *this;
    }
    operator nlohmann::json() const { return data_; }

   private:
    nlohmann::json data_;
};

template <typename T, std::enable_if_t<detail::is_primitive_v<T> ||
                                           detail::is_optional_v<T>,
                                       bool> = true>
void parse(const nlohmann::json& data, const std::string& key, T* value) {
    using Type = std::conditional_t<detail::is_optional_v<T>,
                                    typename detail::is_optional<T>::type, T>;
    using FixedType =
        std::conditional_t<std::is_floating_point_v<Type>, double, Type>;
    using MoreFixedType =
        std::conditional_t<std::is_integral_v<Type>, int64_t, FixedType>;
    using FinalType =
        std::conditional_t<std::is_same_v<Type, bool>, bool, MoreFixedType>;
    if (data.contains(key) && !data[key].is_null()) {
        if constexpr (detail::is_primitive_v<Type>) {
            *value = static_cast<Type>(data[key].get<FinalType>());
        } else {
            *value = parse<typename detail::is_shared_ptr<Type>::type>(data[key]);
        }
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
    result->chat = parseRequired<Chat>(data, "chat");
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
        result->directMessagesTopic = parse<DirectMessagesTopic>(data, "direct_messages_topic");
    parse(data, "sender_tag", &result->senderTag);
    parse(data, "reply_to_checklist_task_id", &result->replyToChecklistTaskId);
    parse(data, "is_paid_post", &result->isPaidPost);
    parse(data, "paid_star_count", &result->paidStarCount);
    result->suggestedPostInfo = parse<SuggestedPostInfo>(data, "suggested_post_info");
    parse(data, "effect_id", &result->effectId);
    result->paidMedia = parse<PaidMediaInfo>(data, "paid_media");
    parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
    result->checklist = parse<Checklist>(data, "checklist");
    result->chatOwnerLeft = parse<ChatOwnerLeft>(data, "chat_owner_left");
    result->chatOwnerChanged = parse<ChatOwnerChanged>(data, "chat_owner_changed");
    result->refundedPayment = parse<RefundedPayment>(data, "refunded_payment");
    result->gift = parse<GiftInfo>(data, "gift");
    result->uniqueGift = parse<UniqueGiftInfo>(data, "unique_gift");
    result->giftUpgradeSent = parse<GiftInfo>(data, "gift_upgrade_sent");
    result->chatBackgroundSet = parse<ChatBackground>(data, "chat_background_set");
    result->checklistTasksDone = parse<ChecklistTasksDone>(data, "checklist_tasks_done");
    result->checklistTasksAdded = parse<ChecklistTasksAdded>(data, "checklist_tasks_added");
    result->directMessagePriceChanged = parse<DirectMessagePriceChanged>(data, "direct_message_price_changed");
    result->paidMessagePriceChanged = parse<PaidMessagePriceChanged>(data, "paid_message_price_changed");
    result->suggestedPostApproved = parse<SuggestedPostApproved>(data, "suggested_post_approved");
    result->suggestedPostApprovalFailed = parse<SuggestedPostApprovalFailed>(data, "suggested_post_approval_failed");
    result->suggestedPostDeclined = parse<SuggestedPostDeclined>(data, "suggested_post_declined");
    result->suggestedPostPaid = parse<SuggestedPostPaid>(data, "suggested_post_paid");
    result->suggestedPostRefunded = parse<SuggestedPostRefunded>(data, "suggested_post_refunded");
    return result;
}

DECLARE_PARSER_TO_JSON(Message) {
    JsonWrapper json;

    if (object) {
        // Simple fields
        json.put("message_id", object->messageId);
        json.put("message_thread_id", object->messageThreadId);
        json.put("from", object->from);
        json.put("sender_chat", object->senderChat);
        json.put("sender_boost_count", object->senderBoostCount);
        json.put("sender_business_bot", object->senderBusinessBot);
        json.put("date", object->date);
        json.put("business_connection_id", object->businessConnectionId);
        json.put("chat", object->chat);
        json.put("forward_origin", object->forwardOrigin);
        json.put("is_topic_message", object->isTopicMessage);
        json.put("is_automatic_forward", object->isAutomaticForward);
        json.put("reply_to_message", object->replyToMessage);
        json.put("external_reply", object->externalReply);
        json.put("quote", object->quote);
        json.put("reply_to_story", object->replyToStory);
        json.put("via_bot", object->viaBot);
        json.put("edit_date", object->editDate);
        json.put("has_protected_content", object->hasProtectedContent);
        json.put("is_from_offline", object->isFromOffline);
        json.put("media_group_id", object->mediaGroupId);
        json.put("author_signature", object->authorSignature);
        json.put("text", object->text);
        json.put("entities", object->entities);
        json.put("link_preview_options", object->linkPreviewOptions);
        json.put("animation", object->animation);
        json.put("audio", object->audio);
        json.put("document", object->document);
        json.put("photo", object->photo);
        json.put("sticker", object->sticker);
        json.put("story", object->story);
        json.put("video", object->video);
        json.put("video_note", object->videoNote);
        json.put("voice", object->voice);
        json.put("caption", object->caption);
        json.put("caption_entities", object->captionEntities);
        json.put("has_media_spoiler", object->hasMediaSpoiler);
        json.put("contact", object->contact);
        json.put("dice", object->dice);
        json.put("game", object->game);
        json.put("poll", object->poll);
        json.put("venue", object->venue);
        json.put("location", object->location);
        json.put("new_chat_members", object->newChatMembers);
        json.put("left_chat_member", object->leftChatMember);
        json.put("new_chat_title", object->newChatTitle);
        json.put("new_chat_photo", object->newChatPhoto);
        json.put("delete_chat_photo", object->deleteChatPhoto);
        json.put("group_chat_created", object->groupChatCreated);
        json.put("supergroup_chat_created", object->supergroupChatCreated);
        json.put("channel_chat_created", object->channelChatCreated);
        json.put("message_auto_delete_timer_changed", object->messageAutoDeleteTimerChanged);
        json.put("migrate_to_chat_id", object->migrateToChatId);
        json.put("migrate_from_chat_id", object->migrateFromChatId);
        json.put("pinned_message", object->pinnedMessage);
        json.put("invoice", object->invoice);
        json.put("successful_payment", object->successfulPayment);
        json.put("users_shared", object->usersShared);
        json.put("chat_shared", object->chatShared);
        json.put("connected_website", object->connectedWebsite);
        json.put("write_access_allowed", object->writeAccessAllowed);
        json.put("passport_data", object->passportData);
        json.put("proximity_alert_triggered", object->proximityAlertTriggered);
        json.put("boost_added", object->boostAdded);
        json.put("forum_topic_created", object->forumTopicCreated);
        json.put("forum_topic_edited", object->forumTopicEdited);
        json.put("forum_topic_closed", object->forumTopicClosed);
        json.put("forum_topic_reopened", object->forumTopicReopened);
        json.put("general_forum_topic_hidden", object->generalForumTopicHidden);
        json.put("general_forum_topic_unhidden", object->generalForumTopicUnhidden);
        json.put("giveaway_created", object->giveawayCreated);
        json.put("giveaway", object->giveaway);
        json.put("giveaway_winners", object->giveawayWinners);
        json.put("giveaway_completed", object->giveawayCompleted);
        json.put("video_chat_scheduled", object->videoChatScheduled);
        json.put("video_chat_started", object->videoChatStarted);
        json.put("video_chat_ended", object->videoChatEnded);
        json.put("video_chat_participants_invited", object->videoChatParticipantsInvited);
        json.put("web_app_data", object->webAppData);
        json.put("reply_markup", object->replyMarkup);
            json.put("direct_messages_topic", object->directMessagesTopic);
        json.put("sender_tag", object->senderTag);
        json.put("reply_to_checklist_task_id", object->replyToChecklistTaskId);
        json.put("is_paid_post", object->isPaidPost);
        json.put("paid_star_count", object->paidStarCount);
        json.put("suggested_post_info", object->suggestedPostInfo);
        json.put("effect_id", object->effectId);
        json.put("paid_media", object->paidMedia);
        json.put("show_caption_above_media", object->showCaptionAboveMedia);
        json.put("checklist", object->checklist);
        json.put("chat_owner_left", object->chatOwnerLeft);
        json.put("chat_owner_changed", object->chatOwnerChanged);
        json.put("refunded_payment", object->refundedPayment);
        json.put("gift", object->gift);
        json.put("unique_gift", object->uniqueGift);
        json.put("gift_upgrade_sent", object->giftUpgradeSent);
        json.put("chat_background_set", object->chatBackgroundSet);
        json.put("checklist_tasks_done", object->checklistTasksDone);
        json.put("checklist_tasks_added", object->checklistTasksAdded);
        json.put("direct_message_price_changed", object->directMessagePriceChanged);
        json.put("paid_message_price_changed", object->paidMessagePriceChanged);
        json.put("suggested_post_approved", object->suggestedPostApproved);
        json.put("suggested_post_approval_failed", object->suggestedPostApprovalFailed);
        json.put("suggested_post_declined", object->suggestedPostDeclined);
        json.put("suggested_post_paid", object->suggestedPostPaid);
        json.put("suggested_post_refunded", object->suggestedPostRefunded);
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
        result->purchasedPaidMedia = parse<PaidMediaPurchased>(data, "purchased_paid_media");
    return result;
}

DECLARE_PARSER_TO_JSON(Update) {
    JsonWrapper json;
    if (object) {
        json.put("update_id", object->updateId);
        json.put("message", object->message);
        json.put("edited_message", object->editedMessage);
        json.put("channel_post", object->channelPost);
        json.put("edited_channel_post", object->editedChannelPost);
        json.put("business_connection", object->businessConnection);
        json.put("business_message", object->businessMessage);
        json.put("edited_business_message", object->editedBusinessMessage);
        json.put("deleted_business_messages", object->deletedBusinessMessages);
        json.put("message_reaction", object->messageReaction);
        json.put("message_reaction_count", object->messageReactionCount);
        json.put("inline_query", object->inlineQuery);
        json.put("chosen_inline_result", object->chosenInlineResult);
        json.put("callback_query", object->callbackQuery);
        json.put("shipping_query", object->shippingQuery);
        json.put("pre_checkout_query", object->preCheckoutQuery);
        json.put("poll", object->poll);
        json.put("poll_answer", object->pollAnswer);
        json.put("my_chat_member", object->myChatMember);
        json.put("chat_member", object->chatMember);
        json.put("chat_join_request", object->chatJoinRequest);
        json.put("chat_boost", object->chatBoost);
        json.put("removed_chat_boost", object->removedChatBoost);
            json.put("purchased_paid_media", object->purchasedPaidMedia);
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
    if (data.contains("allowed_updates") &&
        !data["allowed_updates"].is_null()) {
        result->allowedUpdates =
            parsePrimitiveArray<std::string>(data, "allowed_updates");
    }
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

        json.put("allowed_updates", object->allowedUpdates);
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
        parse(data, "has_main_web_app", &result->hasMainWebApp);
    parse(data, "has_topics_enabled", &result->hasTopicsEnabled);
    parse(data, "allows_users_to_create_topics", &result->allowsUsersToCreateTopics);
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
            json.put("has_main_web_app", object->hasMainWebApp);
        json.put("has_topics_enabled", object->hasTopicsEnabled);
        json.put("allows_users_to_create_topics", object->allowsUsersToCreateTopics);
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
        parse(data, "is_direct_messages", &result->isDirectMessages);
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
        json.put("photo", object->photo);
        json.put("active_usernames", object->activeUsernames);
        json.put("birthdate", object->birthdate);
        json.put("business_intro", object->businessIntro);
        json.put("business_location", object->businessLocation);
        json.put("business_opening_hours", object->businessOpeningHours);
        json.put("personal_chat", object->personalChat);

        json.put("available_reactions", object->availableReactions);

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
        json.put("pinned_message", object->pinnedMessage);
        json.put("permissions", object->permissions);
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
        json.put("location", object->location);
            json.put("is_direct_messages", object->isDirectMessages);
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
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    // Always 0, omit this.
    // parse(data, "date", &result->date);
    return result;
}

DECLARE_PARSER_TO_JSON(InaccessibleMessage) {
    JsonWrapper json;

    if (object) {
        json.put("message_id", object->messageId);
        json.put("date", object->date);
        json.put("chat", object->chat);
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
        parse(data, "unix_time", &result->unixTime);
    parse(data, "date_time_format", &result->dateTimeFormat);
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
        json.put("user", object->user);
        json.put("language", object->language);
        json.put("custom_emoji_id", object->customEmojiId);
            json.put("unix_time", object->unixTime);
        json.put("date_time_format", object->dateTimeFormat);
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
        json.put("entities", object->entities);
        json.put("position", object->position);
        json.put("is_manual", object->isManual);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(ExternalReplyInfo) {
    auto result(std::make_shared<ExternalReplyInfo>());
    result->origin = parseRequired<MessageOrigin>(data, "origin");
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
        result->paidMedia = parse<PaidMediaInfo>(data, "paid_media");
    result->checklist = parse<Checklist>(data, "checklist");
    return result;
}

DECLARE_PARSER_TO_JSON(ExternalReplyInfo) {
    JsonWrapper json;

    if (object) {
        json.put("origin", object->origin);
        json.put("chat", object->chat);
        json.put("message_id", object->messageId);
        json.put("link_preview_options", object->linkPreviewOptions);
        json.put("animation", object->animation);
        json.put("audio", object->audio);
        json.put("document", object->document);
        json.put("photo", object->photo);
        json.put("sticker", object->sticker);
        json.put("story", object->story);
        json.put("video", object->video);
        json.put("video_note", object->videoNote);
        json.put("voice", object->voice);
        json.put("has_media_spoiler", object->hasMediaSpoiler);
        json.put("contact", object->contact);
        json.put("dice", object->dice);
        json.put("game", object->game);
        json.put("giveaway", object->giveaway);
        json.put("giveaway_winners", object->giveawayWinners);
        json.put("invoice", object->invoice);
        json.put("location", object->location);
        json.put("poll", object->poll);
        json.put("venue", object->venue);
            json.put("paid_media", object->paidMedia);
        json.put("checklist", object->checklist);
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
    if (data.contains("quote_entities")) {
        result->quoteEntities =
            parseArray<MessageEntity>(data, "quote_entities");
    }
    parse(data, "quote_position", &result->quotePosition);
        parse(data, "checklist_task_id", &result->checklistTaskId);
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
        json.put("quote_entities", object->quoteEntities);
        json.put("quote_position", object->quotePosition);
            json.put("checklist_task_id", object->checklistTaskId);
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
    result->senderUser = parseRequired<User>(data, "sender_user");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageOriginUser) {
    JsonWrapper json;

    if (object) {
        json.put("sender_user", object->senderUser);
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
    result->senderChat = parseRequired<Chat>(data, "sender_chat");
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
    result->chat = parseRequired<Chat>(data, "chat");
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
        json.put("thumbnail", object->thumbnail);
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
        json.put("thumbnail", object->thumbnail);
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
        json.put("thumbnail", object->thumbnail);
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }

    return json;
}

DECLARE_PARSER_FROM_JSON(Story) {
    auto result(std::make_shared<Story>());
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "id", &result->id);
    return result;
}

DECLARE_PARSER_TO_JSON(Story) {
    JsonWrapper json;

    if (object) {
        json.put("chat", object->chat);
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
        if (data.contains("cover")) result->cover = parseArray<PhotoSize>(data, "cover");
    parse(data, "start_timestamp", &result->startTimestamp);
    if (data.contains("qualities")) result->qualities = parseArray<VideoQuality>(data, "qualities");
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
        json.put("thumbnail", object->thumbnail);
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
            json.put("cover", object->cover);
        json.put("start_timestamp", object->startTimestamp);
        json.put("qualities", object->qualities);
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
        json.put("thumbnail", object->thumbnail);
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
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    return result;
}

DECLARE_PARSER_TO_JSON(PollOption) {
    JsonWrapper json;

    if (object) {
        json.put("text", object->text);
        json.put("voter_count", object->voterCount);
        json.put("text_entities", object->textEntities);
    }

    return json;
}
DECLARE_PARSER_FROM_JSON(PollAnswer) {
    auto result(std::make_shared<PollAnswer>());
    parse(data, "poll_id", &result->pollId);
    result->voterChat = parse<Chat>(data, "voter_chat");
    result->user = parse<User>(data, "user");
    result->optionIds = parsePrimitiveRequiredArray<std::int32_t>(data, "option_ids");
    return result;
}

DECLARE_PARSER_TO_JSON(PollAnswer) {
    JsonWrapper json;
    if (object) {
        json.put("poll_id", object->pollId);
        json.put("voter_chat", object->voterChat);
        json.put("user", object->user);
        json.put("option_ids", object->optionIds);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Poll) {
    auto result(std::make_shared<Poll>());
    parse(data, "id", &result->id);
    parse(data, "question", &result->question);
    result->options = parseRequiredArray<PollOption>(data, "options");
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
        if (data.contains("question_entities")) result->questionEntities = parseArray<MessageEntity>(data, "question_entities");
    return result;
}

DECLARE_PARSER_TO_JSON(Poll) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("question", object->question);
        json.put("options", object->options);
        json.put("total_voter_count", object->totalVoterCount);
        json.put("is_closed", object->isClosed);
        json.put("is_anonymous", object->isAnonymous);
        json.put("type", object->type);
        json.put("allows_multiple_answers", object->allowsMultipleAnswers);
        json.put("correct_option_id", object->correctOptionId);
        json.put("explanation", object->explanation);
        json.put("explanation_entities", object->explanationEntities);
        json.put("open_period", object->openPeriod);
        json.put("close_date", object->closeDate);
            json.put("question_entities", object->questionEntities);
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
    result->location = parseRequired<Location>(data, "location");
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
        json.put("location", object->location);
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
    result->traveler = parseRequired<User>(data, "traveler");
    result->watcher = parseRequired<User>(data, "watcher");
    parse(data, "distance", &result->distance);
    return result;
}

DECLARE_PARSER_TO_JSON(ProximityAlertTriggered) {
    JsonWrapper json;
    if (object) {
        json.put("traveler", object->traveler);
        json.put("watcher", object->watcher);
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
        parse(data, "is_name_implicit", &result->isNameImplicit);
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopicCreated) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("icon_color", object->iconColor);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
            json.put("is_name_implicit", object->isNameImplicit);
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
        json.put("photo", object->photo);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UsersShared) {
    auto result = std::make_shared<UsersShared>();
    parse(data, "request_id", &result->requestId);
    result->users = parseRequiredArray<SharedUser>(data, "users");
    return result;
}

DECLARE_PARSER_TO_JSON(UsersShared) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("users", object->users);
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
        json.put("photo", object->photo);
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
    result->users = parseRequiredArray<User>(data, "users");
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatParticipantsInvited) {
    JsonWrapper json;
    if (object) {
        json.put("users", object->users);
    }
    return json;
}

// GiveawayCreated Parser
DECLARE_PARSER_FROM_JSON(GiveawayCreated) {
    auto result = std::make_shared<GiveawayCreated>();
    parse(data, "prize_star_count", &result->prizeStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayCreated) {
    JsonWrapper json;
    if (object) {
        json.put("prize_star_count", object->prizeStarCount);
    }
    return json;
}

// Giveaway Parser
DECLARE_PARSER_FROM_JSON(Giveaway) {
    auto result = std::make_shared<Giveaway>();
    result->chats = parseRequiredArray<Chat>(data, "chats");
    parse(data, "winners_selection_date", &result->winnersSelectionDate);
    parse(data, "winner_count", &result->winnerCount);
    parse(data, "only_new_members", &result->onlyNewMembers);
    parse(data, "has_public_winners", &result->hasPublicWinners);
    parse(data, "prize_description", &result->prizeDescription);
    result->countryCodes =
        parsePrimitiveArray<std::string>(data, "country_codes");
    parse(data, "premium_subscription_month_count",
          &result->premiumSubscriptionMonthCount);
        parse(data, "prize_star_count", &result->prizeStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(Giveaway) {
    JsonWrapper json;
    if (object) {
        json.put("chats", object->chats);
        json.put("winners_selection_date", object->winnersSelectionDate);
        json.put("winner_count", object->winnerCount);
        json.put("only_new_members", object->onlyNewMembers);
        json.put("has_public_winners", object->hasPublicWinners);
        json.put("prize_description", object->prizeDescription);
        json.put("country_codes", object->countryCodes);
        json.put("premium_subscription_month_count",
                 object->premiumSubscriptionMonthCount);
        json.put("prize_star_count", object->prizeStarCount);
    }
    return json;
}

// GiveawayWinners Parser
DECLARE_PARSER_FROM_JSON(GiveawayWinners) {
    auto result = std::make_shared<GiveawayWinners>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "giveaway_message_id", &result->giveawayMessageId);
    parse(data, "winners_selection_date", &result->winnersSelectionDate);
    parse(data, "winner_count", &result->winnerCount);
    result->winners = parseRequiredArray<User>(data, "winners");
    parse(data, "additional_chat_count", &result->additionalChatCount);
    parse(data, "premium_subscription_month_count",
          &result->premiumSubscriptionMonthCount);
    parse(data, "unclaimed_prize_count", &result->unclaimedPrizeCount);
    parse(data, "only_new_members", &result->onlyNewMembers);
    parse(data, "was_refunded", &result->wasRefunded);
    parse(data, "prize_description", &result->prizeDescription);
        parse(data, "prize_star_count", &result->prizeStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayWinners) {
    JsonWrapper json;
    if (object) {
        json.put("giveaway_message_id", object->giveawayMessageId);
        json.put("winners_selection_date", object->winnersSelectionDate);
        json.put("winner_count", object->winnerCount);
        json.put("winners", object->winners);
        json.put("additional_chat_count", object->additionalChatCount);
        json.put("premium_subscription_month_count",
                 object->premiumSubscriptionMonthCount);
        json.put("unclaimed_prize_count", object->unclaimedPrizeCount);
        json.put("only_new_members", object->onlyNewMembers);
        json.put("was_refunded", object->wasRefunded);
        json.put("prize_description", object->prizeDescription);
            json.put("prize_star_count", object->prizeStarCount);
    }
    return json;
}
// GiveawayCompleted Parser
DECLARE_PARSER_FROM_JSON(GiveawayCompleted) {
    auto result = std::make_shared<GiveawayCompleted>();
    parse(data, "winner_count", &result->winnerCount);
    parse(data, "unclaimed_prize_count", &result->unclaimedPrizeCount);
    result->giveawayMessage = parse<Message>(data, "giveaway_message");
        parse(data, "is_star_giveaway", &result->isStarGiveaway);
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayCompleted) {
    JsonWrapper json;
    if (object) {
        json.put("winner_count", object->winnerCount);
        json.put("unclaimed_prize_count", object->unclaimedPrizeCount);
        json.put("giveaway_message", object->giveawayMessage);
            json.put("is_star_giveaway", object->isStarGiveaway);
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
        json.put("photos", object->photos);
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
        json.put("keyboard", object->keyboard);
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
        parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
    parse(data, "style", &result->style);
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButton) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("request_users", object->requestUsers);
        json.put("request_chat", object->requestChat);
        json.put("request_contact", object->requestContact);
        json.put("request_location", object->requestLocation);
        json.put("request_poll", object->requestPoll);
        json.put("web_app", object->webApp);
            json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
        json.put("style", object->style);
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
    json.put("user_administrator_rights", object->userAdministratorRights);
    json.put("bot_administrator_rights", object->botAdministratorRights);
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
    json.put("inline_keyboard", object->inlineKeyboard);
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
    result->creator = parseRequired<User>(data, "creator");
    parse(data, "creates_join_request", &result->createsJoinRequest);
    parse(data, "is_primary", &result->isPrimary);
    parse(data, "is_revoked", &result->isRevoked);
    parse(data, "name", &result->name);
    parse(data, "expire_date", &result->expireDate);
    parse(data, "member_limit", &result->memberLimit);
    parse(data, "pending_join_request_count", &result->pendingJoinRequestCount);
        parse(data, "subscription_period", &result->subscriptionPeriod);
    parse(data, "subscription_price", &result->subscriptionPrice);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatInviteLink) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("invite_link", object->inviteLink);
    json.put("creator", object->creator);
    json.put("creates_join_request", object->createsJoinRequest);
    json.put("is_primary", object->isPrimary);
    json.put("is_revoked", object->isRevoked);
    json.put("name", object->name);
    json.put("expire_date", object->expireDate);
    json.put("member_limit", object->memberLimit);
    json.put("pending_join_request_count", object->pendingJoinRequestCount);
        json.put("subscription_period", object->subscriptionPeriod);
    json.put("subscription_price", object->subscriptionPrice);
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
        parse(data, "can_manage_direct_messages", &result->canManageDirectMessages);
    parse(data, "can_manage_tags", &result->canManageTags);
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
        json.put("can_manage_direct_messages", object->canManageDirectMessages);
    json.put("can_manage_tags", object->canManageTags);
    return json;
}
DECLARE_PARSER_FROM_JSON(ChatMemberUpdated) {
    auto result = std::make_shared<ChatMemberUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    result->from = parseRequired<User>(data, "from");
    parse(data, "date", &result->date);
    result->oldChatMember = parseRequired<ChatMember>(data, "old_chat_member");
    result->newChatMember = parseRequired<ChatMember>(data, "new_chat_member");
    result->inviteLink = parse<ChatInviteLink>(data, "invite_link");
    parse(data, "via_chat_folder_invite_link",
          &result->viaChatFolderInviteLink);
        parse(data, "via_join_request", &result->viaJoinRequest);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberUpdated) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("chat", object->chat);
    json.put("from", object->from);
    json.put("date", object->date);
    json.put("old_chat_member", object->oldChatMember);
    json.put("new_chat_member", object->newChatMember);
    json.put("invite_link", object->inviteLink);
    json.put("via_chat_folder_invite_link", object->viaChatFolderInviteLink);
        json.put("via_join_request", object->viaJoinRequest);
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
    result->user = parseRequired<User>(data, "user");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMember) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("status", object->status);
    json.put("user", object->user);

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
        parse(data, "can_manage_direct_messages", &result->canManageDirectMessages);
    parse(data, "can_manage_tags", &result->canManageTags);
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
        json.put("can_manage_direct_messages", object->canManageDirectMessages);
    json.put("can_manage_tags", object->canManageTags);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatMemberMember) {
    auto result = std::make_shared<ChatMemberMember>();
    parse(data, "tag", &result->tag);
    parse(data, "until_date", &result->untilDate);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatMemberMember) {
    JsonWrapper json;
    if (object) {
        json.put("tag", object->tag);
        json.put("until_date", object->untilDate);
    }
    return json;
}

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
        parse(data, "tag", &result->tag);
    parse(data, "can_edit_tag", &result->canEditTag);
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
        json.put("tag", object->tag);
    json.put("can_edit_tag", object->canEditTag);
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
    result->chat = parseRequired<Chat>(data, "chat");
    result->from = parseRequired<User>(data, "from");
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
    json.put("chat", object->chat);
    json.put("from", object->from);
    json.put("user_chat_id", object->userChatId);
    json.put("date", object->date);
    json.put("bio", object->bio);
    json.put("invite_link", object->inviteLink);
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
        parse(data, "can_edit_tag", &result->canEditTag);
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
        json.put("can_edit_tag", object->canEditTag);
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
    json.put("sticker", object->sticker);
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
    json.put("location", object->location);
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
        parseRequiredArray<BusinessOpeningHoursInterval>(data, "opening_hours");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessOpeningHours) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("time_zone_name", object->timeZoneName);
    json.put("opening_hours", object->openingHours);
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatLocation) {
    auto result = std::make_shared<ChatLocation>();
    result->location = parseRequired<Location>(data, "location");
    parse(data, "address", &result->address);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatLocation) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("location", object->location);
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
    result->type = parseRequired<ReactionType>(data, "type");
    parse(data, "total_count", &result->totalCount);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionCount) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("type", object->type);
    json.put("total_count", object->totalCount);
    return json;
}

DECLARE_PARSER_FROM_JSON(MessageReactionUpdated) {
    auto result = std::make_shared<MessageReactionUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    result->user = parse<User>(data, "user");
    result->actorChat = parse<Chat>(data, "actor_chat");
    parse(data, "date", &result->date);
    result->oldReaction = parseRequiredArray<ReactionType>(data, "old_reaction");
    result->newReaction = parseRequiredArray<ReactionType>(data, "new_reaction");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionUpdated) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("chat", object->chat);
    json.put("message_id", object->messageId);
    json.put("user", object->user);
    json.put("actor_chat", object->actorChat);
    json.put("date", object->date);
    json.put("old_reaction", object->oldReaction);
    json.put("new_reaction", object->newReaction);
    return json;
}

DECLARE_PARSER_FROM_JSON(MessageReactionCountUpdated) {
    auto result = std::make_shared<MessageReactionCountUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    parse(data, "date", &result->date);
    result->reactions = parseRequiredArray<ReactionCount>(data, "reactions");
    return result;
}

DECLARE_PARSER_TO_JSON(MessageReactionCountUpdated) {
    JsonWrapper json;
    if (!object) {
        return json;
    }
    json.put("chat", object->chat);
    json.put("message_id", object->messageId);
    json.put("date", object->date);
    json.put("reactions", object->reactions);
    return json;
}

DECLARE_PARSER_FROM_JSON(ForumTopic) {
    auto result = std::make_shared<ForumTopic>();
    parse(data, "message_thread_id", &result->messageThreadId);
    parse(data, "name", &result->name);
    parse(data, "icon_color", &result->iconColor);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
        parse(data, "is_name_implicit", &result->isNameImplicit);
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopic) {
    JsonWrapper ptree;
    if (object) {
        ptree.put("message_thread_id", object->messageThreadId);
        ptree.put("name", object->name);
        ptree.put("icon_color", object->iconColor);
        ptree.put("icon_custom_emoji_id", object->iconCustomEmojiId);
            ptree.put("is_name_implicit", object->isNameImplicit);
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
    ptree.put("user", object->user);

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
    result->user = parseRequired<User>(data, "user");
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
    ptree.put("source", object->source);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoost) {
    auto result = std::make_shared<ChatBoost>();
    parse(data, "boost_id", &result->boostId);
    parse(data, "add_date", &result->addDate);
    parse(data, "expiration_date", &result->expirationDate);
    result->source = parseRequired<ChatBoostSource>(data, "source");
    return result;
}

// ChatBoostUpdated Parsing
DECLARE_PARSER_TO_JSON(ChatBoostUpdated) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("chat", object->chat);
    ptree.put("boost", object->boost);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoostUpdated) {
    auto result = std::make_shared<ChatBoostUpdated>();
    result->chat = parseRequired<Chat>(data, "chat");
    result->boost = parseRequired<ChatBoost>(data, "boost");
    return result;
}

// ChatBoostRemoved Parsing
DECLARE_PARSER_TO_JSON(ChatBoostRemoved) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("chat", object->chat);
    ptree.put("boost_id", object->boostId);
    ptree.put("remove_date", object->removeDate);
    ptree.put("source", object->source);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoostRemoved) {
    auto result = std::make_shared<ChatBoostRemoved>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "boost_id", &result->boostId);
    parse(data, "remove_date", &result->removeDate);
    result->source = parseRequired<ChatBoostSource>(data, "source");
    return result;
}

// UserChatBoosts Parsing
DECLARE_PARSER_TO_JSON(UserChatBoosts) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("boosts", object->boosts);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(UserChatBoosts) {
    auto result = std::make_shared<UserChatBoosts>();
    result->boosts = parseRequiredArray<ChatBoost>(data, "boosts");
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
    ptree.put("web_app", object->webApp);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(MenuButtonWebApp) {
    auto result = std::make_shared<MenuButtonWebApp>();
    parse(data, "text", &result->text);
    result->webApp = parseRequired<WebAppInfo>(data, "web_app");
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
        ptree.put("prize_star_count", object->prizeStarCount);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(ChatBoostSourceGiveaway) {
    auto result = std::make_shared<ChatBoostSourceGiveaway>();
    parse(data, "giveaway_message_id", &result->giveawayMessageId);
    parse(data, "is_unclaimed", &result->isUnclaimed);
        parse(data, "prize_star_count", &result->prizeStarCount);
    return result;
}

// BusinessConnection Parsing
DECLARE_PARSER_TO_JSON(BusinessConnection) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("id", object->id);
    ptree.put("user", object->user);
    ptree.put("user_chat_id", object->userChatId);
    ptree.put("date", object->date);
    ptree.put("can_reply", object->canReply);
    ptree.put("is_enabled", object->isEnabled);
        ptree.put("rights", object->rights);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BusinessConnection) {
    auto result = std::make_shared<BusinessConnection>();
    parse(data, "id", &result->id);
    result->user = parseRequired<User>(data, "user");
    parse(data, "user_chat_id", &result->userChatId);
    parse(data, "date", &result->date);
    parse(data, "can_reply", &result->canReply);
    parse(data, "is_enabled", &result->isEnabled);
        result->rights = parse<BusinessBotRights>(data, "rights");
    return result;
}

// BusinessMessagesDeleted Parsing
DECLARE_PARSER_TO_JSON(BusinessMessagesDeleted) {
    JsonWrapper ptree;
    if (!object) return ptree;
    ptree.put("business_connection_id", object->businessConnectionId);
    ptree.put("chat", object->chat);
    ptree.put("message_ids", object->messageIds);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(BusinessMessagesDeleted) {
    auto result = std::make_shared<BusinessMessagesDeleted>();
    parse(data, "business_connection_id", &result->businessConnectionId);
    result->chat = parseRequired<Chat>(data, "chat");
    result->messageIds = parsePrimitiveRequiredArray<std::int32_t>(data, "message_ids");
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
    ptree.put("caption_entities", object->captionEntities);

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
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMediaPhoto) {
    auto result = std::make_shared<InputMediaPhoto>();
    parse(data, "has_spoiler", &result->hasSpoiler);
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
        ptree.put("cover", object->cover);
    ptree.put("start_timestamp", object->startTimestamp);
    ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
        parse(data, "cover", &result->cover);
    parse(data, "start_timestamp", &result->startTimestamp);
    parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
    return ptree;
}

DECLARE_PARSER_FROM_JSON(InputMediaAnimation) {
    auto result = std::make_shared<InputMediaAnimation>();
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    parse(data, "has_spoiler", &result->hasSpoiler);
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("thumbnail", object->thumbnail);
    ptree.put("emoji", object->emoji);
    ptree.put("set_name", object->setName);
    ptree.put("premium_animation", object->premiumAnimation);
    ptree.put("mask_position", object->maskPosition);
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
    result->stickers = parseRequiredArray<Sticker>(data, "stickers");
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
    ptree.put("stickers", object->stickers);
    ptree.put("thumbnail", object->thumbnail);
    return ptree;
}

// CallbackQuery
DECLARE_PARSER_FROM_JSON(CallbackQuery) {
    auto result = std::make_shared<CallbackQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
    result->message = parse<Message>(data, "message");
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
    ptree.put("from", object->from);
    ptree.put("message", object->message);
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
    result->from = parseRequired<User>(data, "from");
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
    ptree.put("from", object->from);
    ptree.put("query", object->query);
    ptree.put("offset", object->offset);
    ptree.put("chat_type", object->chatType);
    ptree.put("location", object->location);

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
        parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
    parse(data, "style", &result->style);
    result->copyText = parse<CopyTextButton>(data, "copy_text");
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
    ptree.put("web_app", object->webApp);
    ptree.put("login_url", object->loginUrl);
    ptree.put("switch_inline_query", object->switchInlineQuery);
    ptree.put("switch_inline_query_current_chat",
              object->switchInlineQueryCurrentChat);
    ptree.put("switch_inline_query_chosen_chat", object->switchInlineQueryChosenChat);
    ptree.put("callback_game", object->callbackGame);
    ptree.put("pay", object->pay);

        ptree.put("icon_custom_emoji_id", object->iconCustomEmojiId);
    ptree.put("style", object->style);
    ptree.put("copy_text", object->copyText);
    return ptree;
}

template <typename T, typename CachedT>
auto put(const InlineQueryResult::Ptr& ptr) {
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
    ptree.put("reply_markup", object->replyMarkup);

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
    result->emojiList = parsePrimitiveRequiredArray<std::string>(data, "emoji_list");
    result->maskPosition = parse<MaskPosition>(data, "mask_position");
    if (data.contains("keywords") && !data["keywords"].is_null()) {
        result->keywords = parsePrimitiveArray<std::string>(data, "keywords");
    }
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
    ptree.put("emoji_list", object->emojiList);
    ptree.put("mask_position", object->maskPosition);
    ptree.put("keywords", object->keywords);

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
    ptree.put("web_app", object->webApp);
    ptree.put("start_parameter", object->startParameter);

    return ptree;
}

DECLARE_PARSER_FROM_JSON(InlineQueryResultArticle) {
    auto result = std::make_shared<InlineQueryResultArticle>();
    parse(data, "title", &result->title);
    result->inputMessageContent =
        parseRequired<InputMessageContent>(data, "input_message_content");
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
    ptree.put("input_message_content", object->inputMessageContent);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("video_width", object->videoWidth);
    ptree.put("video_height", object->videoHeight);
    ptree.put("video_duration", object->videoDuration);
    ptree.put("description", object->description);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("performer", object->performer);
    ptree.put("audio_duration", object->audioDuration);
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("voice_duration", object->voiceDuration);
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("document_url", object->documentUrl);
    ptree.put("mime_type", object->mimeType);
    ptree.put("description", object->description);
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("input_message_content", object->inputMessageContent);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
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
        parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
        ptree.put("show_caption_above_media", object->showCaptionAboveMedia);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("caption_entities", object->captionEntities);
    ptree.put("input_message_content", object->inputMessageContent);
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
    ptree.put("entities", object->entities);
    ptree.put("link_preview_options", object->linkPreviewOptions);
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
    result->prices = parseRequiredArray<LabeledPrice>(data, "prices");
    parse(data, "max_tip_amount", &result->maxTipAmount);
    if (data.contains("suggested_tip_amounts") &&
        !data["suggested_tip_amounts"].is_null()) {
        result->suggestedTipAmounts =
            parsePrimitiveArray<std::int32_t>(data, "suggested_tip_amounts");
    }
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
    ptree.put("prices", object->prices);
    ptree.put("max_tip_amount", object->maxTipAmount);
    ptree.put("suggested_tip_amounts", object->suggestedTipAmounts);
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
    result->from = parseRequired<User>(data, "from");
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
    ptree.put("from", object->from);
    ptree.put("location", object->location);
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
    ptree.put("shipping_address", object->shippingAddress);
    return ptree;
}

// ShippingOption
DECLARE_PARSER_FROM_JSON(ShippingOption) {
    auto result = std::make_shared<ShippingOption>();
    parse(data, "id", &result->id);
    parse(data, "title", &result->title);
    result->prices = parseRequiredArray<LabeledPrice>(data, "prices");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingOption) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("id", object->id);
    ptree.put("title", object->title);
    ptree.put("prices", object->prices);
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
        parse(data, "subscription_expiration_date", &result->subscriptionExpirationDate);
    parse(data, "is_recurring", &result->isRecurring);
    parse(data, "is_first_recurring", &result->isFirstRecurring);
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
    ptree.put("order_info", object->orderInfo);
    ptree.put("telegram_payment_charge_id", object->telegramPaymentChargeId);
    ptree.put("provider_payment_charge_id", object->providerPaymentChargeId);
        ptree.put("subscription_expiration_date", object->subscriptionExpirationDate);
    ptree.put("is_recurring", object->isRecurring);
    ptree.put("is_first_recurring", object->isFirstRecurring);
    return ptree;
}

// ShippingQuery
DECLARE_PARSER_FROM_JSON(ShippingQuery) {
    auto result = std::make_shared<ShippingQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
    parse(data, "invoice_payload", &result->invoicePayload);
    result->shippingAddress = parseRequired<ShippingAddress>(data, "shipping_address");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingQuery) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("id", object->id);
    ptree.put("from", object->from);
    ptree.put("invoice_payload", object->invoicePayload);
    ptree.put("shipping_address", object->shippingAddress);
    return ptree;
}

// PreCheckoutQuery
DECLARE_PARSER_FROM_JSON(PreCheckoutQuery) {
    auto result = std::make_shared<PreCheckoutQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
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
    ptree.put("from", object->from);
    ptree.put("currency", object->currency);
    ptree.put("total_amount", object->totalAmount);
    ptree.put("invoice_payload", object->invoicePayload);
    ptree.put("shipping_option_id", object->shippingOptionId);
    ptree.put("order_info", object->orderInfo);
    return ptree;
}

// PassportData
DECLARE_PARSER_FROM_JSON(PassportData) {
    auto result = std::make_shared<PassportData>();
    result->data = parseRequiredArray<EncryptedPassportElement>(data, "data");
    result->credentials = parseRequired<EncryptedCredentials>(data, "credentials");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportData) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("data", object->data);
    ptree.put("credentials", object->credentials);
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
    ptree.put("files", object->files);
    ptree.put("front_side", object->frontSide);
    ptree.put("reverse_side", object->reverseSide);
    ptree.put("selfie", object->selfie);
    ptree.put("translation", object->translation);
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
    result->fileHashes = parsePrimitiveRequiredArray<std::string>(data, "file_hashes");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorFiles) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_hashes", object->fileHashes);
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
    result->fileHashes = parsePrimitiveRequiredArray<std::string>(data, "file_hashes");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorTranslationFiles) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("file_hashes", object->fileHashes);
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
    result->photo = parseRequiredArray<PhotoSize>(data, "photo");
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
    ptree.put("photo", object->photo);
    ptree.put("text", object->text);
    ptree.put("text_entities", object->textEntities);
    ptree.put("animation", object->animation);
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
    result->user = parseRequired<User>(data, "user");
    parse(data, "score", &result->score);
    return result;
}

DECLARE_PARSER_TO_JSON(GameHighScore) {
    JsonWrapper ptree;
    if (!object) {
        return ptree;
    }
    ptree.put("position", object->position);
    ptree.put("user", object->user);
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

DECLARE_PARSER_FROM_JSON(AcceptedGiftTypes) {
    auto result(std::make_shared<AcceptedGiftTypes>());
    parse(data, "unlimited_gifts", &result->unlimitedGifts);
    parse(data, "limited_gifts", &result->limitedGifts);
    parse(data, "unique_gifts", &result->uniqueGifts);
    parse(data, "premium_subscription", &result->premiumSubscription);
    parse(data, "gifts_from_channels", &result->giftsFromChannels);
    return result;
}

DECLARE_PARSER_TO_JSON(AcceptedGiftTypes) {
    JsonWrapper json;
    if (object) {
        json.put("unlimited_gifts", object->unlimitedGifts);
        json.put("limited_gifts", object->limitedGifts);
        json.put("unique_gifts", object->uniqueGifts);
        json.put("premium_subscription", object->premiumSubscription);
        json.put("gifts_from_channels", object->giftsFromChannels);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundFillFreeformGradient) {
    auto result(std::make_shared<BackgroundFillFreeformGradient>());
    result->colors = parsePrimitiveRequiredArray<int64_t>(data, "colors");
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundFillFreeformGradient) {
    JsonWrapper json;
    if (object) {
        json.put("colors", object->colors);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundFillGradient) {
    auto result(std::make_shared<BackgroundFillGradient>());
    parse(data, "top_color", &result->topColor);
    parse(data, "bottom_color", &result->bottomColor);
    parse(data, "rotation_angle", &result->rotationAngle);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundFillGradient) {
    JsonWrapper json;
    if (object) {
        json.put("top_color", object->topColor);
        json.put("bottom_color", object->bottomColor);
        json.put("rotation_angle", object->rotationAngle);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundFillSolid) {
    auto result(std::make_shared<BackgroundFillSolid>());
    parse(data, "color", &result->color);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundFillSolid) {
    JsonWrapper json;
    if (object) {
        json.put("color", object->color);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundFill) {
    if (data.contains("colors")) {
        return parse<BackgroundFillFreeformGradient>(data);
    } else if (data.contains("top_color") && data.contains("bottom_color")) {
        return parse<BackgroundFillGradient>(data);
    } else if (data.contains("color")) {
        return parse<BackgroundFillSolid>(data);
    }
    throw invalidType("json", data.dump());
}

DECLARE_PARSER_TO_JSON(BackgroundFill) {
    if (!object) {
        return JsonWrapper{};
    }
    if (object->type == BackgroundFillFreeformGradient::TYPE) {
        return put<BackgroundFillFreeformGradient>(object);
    } else if (object->type == BackgroundFillGradient::TYPE) {
        return put<BackgroundFillGradient>(object);
    } else if (object->type == BackgroundFillSolid::TYPE) {
        return put<BackgroundFillSolid>(object);
    }
    throw invalidType("type", object->type);
}

DECLARE_PARSER_FROM_JSON(BackgroundTypeChatTheme) {
    auto result(std::make_shared<BackgroundTypeChatTheme>());
    parse(data, "theme_name", &result->themeName);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypeChatTheme) {
    JsonWrapper json;
    if (object) {
        json.put("theme_name", object->themeName);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundTypeFill) {
    auto result(std::make_shared<BackgroundTypeFill>());
    result->fill = parseRequired<BackgroundFill>(data, "fill");
    parse(data, "dark_theme_dimming", &result->darkThemeDimming);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypeFill) {
    JsonWrapper json;
    if (object) {
        json.put("fill", object->fill);
        json.put("dark_theme_dimming", object->darkThemeDimming);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundTypePattern) {
    auto result(std::make_shared<BackgroundTypePattern>());
    result->document = parseRequired<Document>(data, "document");
    result->fill = parseRequired<BackgroundFill>(data, "fill");
    parse(data, "intensity", &result->intensity);
    parse(data, "is_inverted", &result->isInverted);
    parse(data, "is_moving", &result->isMoving);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypePattern) {
    JsonWrapper json;
    if (object) {
        json.put("document", object->document);
        json.put("fill", object->fill);
        json.put("intensity", object->intensity);
        json.put("is_inverted", object->isInverted);
        json.put("is_moving", object->isMoving);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundTypeWallpaper) {
    auto result(std::make_shared<BackgroundTypeWallpaper>());
    result->document = parseRequired<Document>(data, "document");
    parse(data, "dark_theme_dimming", &result->darkThemeDimming);
    parse(data, "is_blurred", &result->isBlurred);
    parse(data, "is_moving", &result->isMoving);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypeWallpaper) {
    JsonWrapper json;
    if (object) {
        json.put("document", object->document);
        json.put("dark_theme_dimming", object->darkThemeDimming);
        json.put("is_blurred", object->isBlurred);
        json.put("is_moving", object->isMoving);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(BackgroundType) {
    if (data.contains("theme_name")) {
        return parse<BackgroundTypeChatTheme>(data);
    } else if (data.contains("fill")) {
        return parse<BackgroundTypeFill>(data);
    } else if (data.contains("document") && data.contains("fill")) {
        return parse<BackgroundTypePattern>(data);
    } else if (data.contains("document") && data.contains("dark_theme_dimming")) {
        return parse<BackgroundTypeWallpaper>(data);
    }
    throw invalidType("json", data.dump());
}

DECLARE_PARSER_TO_JSON(BackgroundType) {
    if (!object) {
        return JsonWrapper{};
    }
    if (object->type == BackgroundTypeChatTheme::TYPE) {
        return put<BackgroundTypeChatTheme>(object);
    } else if (object->type == BackgroundTypeFill::TYPE) {
        return put<BackgroundTypeFill>(object);
    } else if (object->type == BackgroundTypePattern::TYPE) {
        return put<BackgroundTypePattern>(object);
    } else if (object->type == BackgroundTypeWallpaper::TYPE) {
        return put<BackgroundTypeWallpaper>(object);
    }
    throw invalidType("type", object->type);
}

DECLARE_PARSER_FROM_JSON(BusinessBotRights) {
    auto result(std::make_shared<BusinessBotRights>());
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

DECLARE_PARSER_FROM_JSON(ChatBackground) {
    auto result(std::make_shared<ChatBackground>());
    result->type = parseRequired<BackgroundType>(data, "type");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBackground) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatFullInfo) {
    auto result(std::make_shared<ChatFullInfo>());
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
    parse(data, "photo", &result->photo);
    result->activeUsernames = parsePrimitiveArray<std::string>(data, "active_usernames");
    parse(data, "birthdate", &result->birthdate);
    parse(data, "business_intro", &result->businessIntro);
    parse(data, "business_location", &result->businessLocation);
    parse(data, "business_opening_hours", &result->businessOpeningHours);
    parse(data, "personal_chat", &result->personalChat);
    parse(data, "parent_chat", &result->parentChat);
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
    parse(data, "pinned_message", &result->pinnedMessage);
    parse(data, "permissions", &result->permissions);
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
    parse(data, "location", &result->location);
    parse(data, "rating", &result->rating);
    parse(data, "first_profile_audio", &result->firstProfileAudio);
    parse(data, "unique_gift_colors", &result->uniqueGiftColors);
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

DECLARE_PARSER_FROM_JSON(ChatOwnerChanged) {
    auto result(std::make_shared<ChatOwnerChanged>());
    result->newOwner = parseRequired<User>(data, "new_owner");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatOwnerChanged) {
    JsonWrapper json;
    if (object) {
        json.put("new_owner", object->newOwner);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChatOwnerLeft) {
    auto result(std::make_shared<ChatOwnerLeft>());
    parse(data, "new_owner", &result->newOwner);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatOwnerLeft) {
    JsonWrapper json;
    if (object) {
        json.put("new_owner", object->newOwner);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Checklist) {
    auto result(std::make_shared<Checklist>());
    parse(data, "title", &result->title);
    result->titleEntities = parseArray<MessageEntity>(data, "title_entities");
    result->tasks = parseRequiredArray<ChecklistTask>(data, "tasks");
    parse(data, "others_can_add_tasks", &result->othersCanAddTasks);
    parse(data, "others_can_mark_tasks_as_done", &result->othersCanMarkTasksAsDone);
    return result;
}

DECLARE_PARSER_TO_JSON(Checklist) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("title_entities", object->titleEntities);
        json.put("tasks", object->tasks);
        json.put("others_can_add_tasks", object->othersCanAddTasks);
        json.put("others_can_mark_tasks_as_done", object->othersCanMarkTasksAsDone);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChecklistTask) {
    auto result(std::make_shared<ChecklistTask>());
    parse(data, "id", &result->id);
    parse(data, "text", &result->text);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    parse(data, "completed_by_user", &result->completedByUser);
    parse(data, "completed_by_chat", &result->completedByChat);
    parse(data, "completion_date", &result->completionDate);
    return result;
}

DECLARE_PARSER_TO_JSON(ChecklistTask) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("text", object->text);
        json.put("text_entities", object->textEntities);
        json.put("completed_by_user", object->completedByUser);
        json.put("completed_by_chat", object->completedByChat);
        json.put("completion_date", object->completionDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChecklistTasksAdded) {
    auto result(std::make_shared<ChecklistTasksAdded>());
    parse(data, "checklist_message", &result->checklistMessage);
    result->tasks = parseRequiredArray<ChecklistTask>(data, "tasks");
    return result;
}

DECLARE_PARSER_TO_JSON(ChecklistTasksAdded) {
    JsonWrapper json;
    if (object) {
        json.put("checklist_message", object->checklistMessage);
        json.put("tasks", object->tasks);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ChecklistTasksDone) {
    auto result(std::make_shared<ChecklistTasksDone>());
    parse(data, "checklist_message", &result->checklistMessage);
    result->markedAsDoneTaskIds = parsePrimitiveRequiredArray<int64_t>(data, "marked_as_done_task_ids");
    result->markedAsNotDoneTaskIds = parsePrimitiveRequiredArray<int64_t>(data, "marked_as_not_done_task_ids");
    return result;
}

DECLARE_PARSER_TO_JSON(ChecklistTasksDone) {
    JsonWrapper json;
    if (object) {
        json.put("checklist_message", object->checklistMessage);
        json.put("marked_as_done_task_ids", object->markedAsDoneTaskIds);
        json.put("marked_as_not_done_task_ids", object->markedAsNotDoneTaskIds);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(CopyTextButton) {
    auto result(std::make_shared<CopyTextButton>());
    parse(data, "text", &result->text);
    return result;
}

DECLARE_PARSER_TO_JSON(CopyTextButton) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(DirectMessagePriceChanged) {
    auto result(std::make_shared<DirectMessagePriceChanged>());
    parse(data, "are_direct_messages_enabled", &result->areDirectMessagesEnabled);
    parse(data, "direct_message_star_count", &result->directMessageStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(DirectMessagePriceChanged) {
    JsonWrapper json;
    if (object) {
        json.put("are_direct_messages_enabled", object->areDirectMessagesEnabled);
        json.put("direct_message_star_count", object->directMessageStarCount);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(DirectMessagesTopic) {
    auto result(std::make_shared<DirectMessagesTopic>());
    parse(data, "topic_id", &result->topicId);
    parse(data, "user", &result->user);
    return result;
}

DECLARE_PARSER_TO_JSON(DirectMessagesTopic) {
    JsonWrapper json;
    if (object) {
        json.put("topic_id", object->topicId);
        json.put("user", object->user);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Gift) {
    auto result(std::make_shared<Gift>());
    parse(data, "id", &result->id);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "star_count", &result->starCount);
    parse(data, "upgrade_star_count", &result->upgradeStarCount);
    parse(data, "is_premium", &result->isPremium);
    parse(data, "has_colors", &result->hasColors);
    parse(data, "total_count", &result->totalCount);
    parse(data, "remaining_count", &result->remainingCount);
    parse(data, "personal_total_count", &result->personalTotalCount);
    parse(data, "personal_remaining_count", &result->personalRemainingCount);
    parse(data, "background", &result->background);
    parse(data, "unique_gift_variant_count", &result->uniqueGiftVariantCount);
    parse(data, "publisher_chat", &result->publisherChat);
    return result;
}

DECLARE_PARSER_TO_JSON(Gift) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("sticker", object->sticker);
        json.put("star_count", object->starCount);
        json.put("upgrade_star_count", object->upgradeStarCount);
        json.put("is_premium", object->isPremium);
        json.put("has_colors", object->hasColors);
        json.put("total_count", object->totalCount);
        json.put("remaining_count", object->remainingCount);
        json.put("personal_total_count", object->personalTotalCount);
        json.put("personal_remaining_count", object->personalRemainingCount);
        json.put("background", object->background);
        json.put("unique_gift_variant_count", object->uniqueGiftVariantCount);
        json.put("publisher_chat", object->publisherChat);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(GiftBackground) {
    auto result(std::make_shared<GiftBackground>());
    parse(data, "center_color", &result->centerColor);
    parse(data, "edge_color", &result->edgeColor);
    parse(data, "text_color", &result->textColor);
    return result;
}

DECLARE_PARSER_TO_JSON(GiftBackground) {
    JsonWrapper json;
    if (object) {
        json.put("center_color", object->centerColor);
        json.put("edge_color", object->edgeColor);
        json.put("text_color", object->textColor);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(GiftInfo) {
    auto result(std::make_shared<GiftInfo>());
    result->gift = parseRequired<Gift>(data, "gift");
    parse(data, "owned_gift_id", &result->ownedGiftId);
    parse(data, "convert_star_count", &result->convertStarCount);
    parse(data, "prepaid_upgrade_star_count", &result->prepaidUpgradeStarCount);
    parse(data, "is_upgrade_separate", &result->isUpgradeSeparate);
    parse(data, "can_be_upgraded", &result->canBeUpgraded);
    parse(data, "text", &result->text);
    result->entities = parseArray<MessageEntity>(data, "entities");
    parse(data, "is_private", &result->isPrivate);
    parse(data, "unique_gift_number", &result->uniqueGiftNumber);
    return result;
}

DECLARE_PARSER_TO_JSON(GiftInfo) {
    JsonWrapper json;
    if (object) {
        json.put("gift", object->gift);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("convert_star_count", object->convertStarCount);
        json.put("prepaid_upgrade_star_count", object->prepaidUpgradeStarCount);
        json.put("is_upgrade_separate", object->isUpgradeSeparate);
        json.put("can_be_upgraded", object->canBeUpgraded);
        json.put("text", object->text);
        json.put("entities", object->entities);
        json.put("is_private", object->isPrivate);
        json.put("unique_gift_number", object->uniqueGiftNumber);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(Gifts) {
    auto result(std::make_shared<Gifts>());
    result->gifts = parseRequiredArray<Gift>(data, "gifts");
    return result;
}

DECLARE_PARSER_TO_JSON(Gifts) {
    JsonWrapper json;
    if (object) {
        json.put("gifts", object->gifts);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputChecklist) {
    auto result(std::make_shared<InputChecklist>());
    parse(data, "title", &result->title);
    parse(data, "parse_mode", &result->parseMode);
    result->titleEntities = parseArray<MessageEntity>(data, "title_entities");
    result->tasks = parseRequiredArray<InputChecklistTask>(data, "tasks");
    parse(data, "others_can_add_tasks", &result->othersCanAddTasks);
    parse(data, "others_can_mark_tasks_as_done", &result->othersCanMarkTasksAsDone);
    return result;
}

DECLARE_PARSER_TO_JSON(InputChecklist) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("parse_mode", object->parseMode);
        json.put("title_entities", object->titleEntities);
        json.put("tasks", object->tasks);
        json.put("others_can_add_tasks", object->othersCanAddTasks);
        json.put("others_can_mark_tasks_as_done", object->othersCanMarkTasksAsDone);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputChecklistTask) {
    auto result(std::make_shared<InputChecklistTask>());
    parse(data, "id", &result->id);
    parse(data, "text", &result->text);
    parse(data, "parse_mode", &result->parseMode);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    return result;
}

DECLARE_PARSER_TO_JSON(InputChecklistTask) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("text", object->text);
        json.put("parse_mode", object->parseMode);
        json.put("text_entities", object->textEntities);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputPaidMediaPhoto) {
    auto result(std::make_shared<InputPaidMediaPhoto>());
    parse(data, "media", &result->media);
    return result;
}

DECLARE_PARSER_TO_JSON(InputPaidMediaPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("media", object->media);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputPaidMediaVideo) {
    auto result(std::make_shared<InputPaidMediaVideo>());
    parse(data, "media", &result->media);
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "cover", &result->cover);
    parse(data, "start_timestamp", &result->startTimestamp);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    parse(data, "supports_streaming", &result->supportsStreaming);
    return result;
}

DECLARE_PARSER_TO_JSON(InputPaidMediaVideo) {
    JsonWrapper json;
    if (object) {
        json.put("media", object->media);
        json.put("thumbnail", object->thumbnail);
        json.put("cover", object->cover);
        json.put("start_timestamp", object->startTimestamp);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
        json.put("supports_streaming", object->supportsStreaming);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputPollOption) {
    auto result(std::make_shared<InputPollOption>());
    parse(data, "text", &result->text);
    parse(data, "text_parse_mode", &result->textParseMode);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    return result;
}

DECLARE_PARSER_TO_JSON(InputPollOption) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("text_parse_mode", object->textParseMode);
        json.put("text_entities", object->textEntities);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputProfilePhoto) {
    auto result(std::make_shared<InputProfilePhoto>());
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(InputProfilePhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputProfilePhotoAnimated) {
    auto result(std::make_shared<InputProfilePhotoAnimated>());
    parse(data, "animation", &result->animation);
    parse(data, "main_frame_timestamp", &result->mainFrameTimestamp);
    return result;
}

DECLARE_PARSER_TO_JSON(InputProfilePhotoAnimated) {
    JsonWrapper json;
    if (object) {
        json.put("animation", object->animation);
        json.put("main_frame_timestamp", object->mainFrameTimestamp);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputProfilePhotoStatic) {
    auto result(std::make_shared<InputProfilePhotoStatic>());
    parse(data, "photo", &result->photo);
    return result;
}

DECLARE_PARSER_TO_JSON(InputProfilePhotoStatic) {
    JsonWrapper json;
    if (object) {
        json.put("photo", object->photo);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputStoryContent) {
    auto result(std::make_shared<InputStoryContent>());
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(InputStoryContent) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputStoryContentPhoto) {
    auto result(std::make_shared<InputStoryContentPhoto>());
    parse(data, "photo", &result->photo);
    return result;
}

DECLARE_PARSER_TO_JSON(InputStoryContentPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("photo", object->photo);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(InputStoryContentVideo) {
    auto result(std::make_shared<InputStoryContentVideo>());
    parse(data, "video", &result->video);
    parse(data, "duration", &result->duration);
    parse(data, "cover_frame_timestamp", &result->coverFrameTimestamp);
    parse(data, "is_animation", &result->isAnimation);
    return result;
}

DECLARE_PARSER_TO_JSON(InputStoryContentVideo) {
    JsonWrapper json;
    if (object) {
        json.put("video", object->video);
        json.put("duration", object->duration);
        json.put("cover_frame_timestamp", object->coverFrameTimestamp);
        json.put("is_animation", object->isAnimation);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(LocationAddress) {
    auto result(std::make_shared<LocationAddress>());
    parse(data, "country_code", &result->countryCode);
    parse(data, "state", &result->state);
    parse(data, "city", &result->city);
    parse(data, "street", &result->street);
    return result;
}

DECLARE_PARSER_TO_JSON(LocationAddress) {
    JsonWrapper json;
    if (object) {
        json.put("country_code", object->countryCode);
        json.put("state", object->state);
        json.put("city", object->city);
        json.put("street", object->street);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(OwnedGift) {
    auto result(std::make_shared<OwnedGift>());
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGift) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(OwnedGiftRegular) {
    auto result(std::make_shared<OwnedGiftRegular>());
    result->gift = parseRequired<Gift>(data, "gift");
    parse(data, "owned_gift_id", &result->ownedGiftId);
    parse(data, "sender_user", &result->senderUser);
    parse(data, "send_date", &result->sendDate);
    parse(data, "text", &result->text);
    result->entities = parseArray<MessageEntity>(data, "entities");
    parse(data, "is_private", &result->isPrivate);
    parse(data, "is_saved", &result->isSaved);
    parse(data, "can_be_upgraded", &result->canBeUpgraded);
    parse(data, "was_refunded", &result->wasRefunded);
    parse(data, "convert_star_count", &result->convertStarCount);
    parse(data, "prepaid_upgrade_star_count", &result->prepaidUpgradeStarCount);
    parse(data, "is_upgrade_separate", &result->isUpgradeSeparate);
    parse(data, "unique_gift_number", &result->uniqueGiftNumber);
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGiftRegular) {
    JsonWrapper json;
    if (object) {
        json.put("gift", object->gift);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("sender_user", object->senderUser);
        json.put("send_date", object->sendDate);
        json.put("text", object->text);
        json.put("entities", object->entities);
        json.put("is_private", object->isPrivate);
        json.put("is_saved", object->isSaved);
        json.put("can_be_upgraded", object->canBeUpgraded);
        json.put("was_refunded", object->wasRefunded);
        json.put("convert_star_count", object->convertStarCount);
        json.put("prepaid_upgrade_star_count", object->prepaidUpgradeStarCount);
        json.put("is_upgrade_separate", object->isUpgradeSeparate);
        json.put("unique_gift_number", object->uniqueGiftNumber);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(OwnedGifts) {
    auto result(std::make_shared<OwnedGifts>());
    parse(data, "total_count", &result->totalCount);
    result->gifts = parseRequiredArray<OwnedGift>(data, "gifts");
    parse(data, "next_offset", &result->nextOffset);
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGifts) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("gifts", object->gifts);
        json.put("next_offset", object->nextOffset);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(OwnedGiftUnique) {
    auto result(std::make_shared<OwnedGiftUnique>());
    result->gift = parseRequired<UniqueGift>(data, "gift");
    parse(data, "owned_gift_id", &result->ownedGiftId);
    parse(data, "sender_user", &result->senderUser);
    parse(data, "send_date", &result->sendDate);
    parse(data, "is_saved", &result->isSaved);
    parse(data, "can_be_transferred", &result->canBeTransferred);
    parse(data, "transfer_star_count", &result->transferStarCount);
    parse(data, "next_transfer_date", &result->nextTransferDate);
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGiftUnique) {
    JsonWrapper json;
    if (object) {
        json.put("gift", object->gift);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("sender_user", object->senderUser);
        json.put("send_date", object->sendDate);
        json.put("is_saved", object->isSaved);
        json.put("can_be_transferred", object->canBeTransferred);
        json.put("transfer_star_count", object->transferStarCount);
        json.put("next_transfer_date", object->nextTransferDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PaidMedia) {
    auto result(std::make_shared<PaidMedia>());
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMedia) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PaidMediaInfo) {
    auto result(std::make_shared<PaidMediaInfo>());
    parse(data, "star_count", &result->starCount);
    result->paidMedia = parseRequiredArray<PaidMedia>(data, "paid_media");
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaInfo) {
    JsonWrapper json;
    if (object) {
        json.put("star_count", object->starCount);
        json.put("paid_media", object->paidMedia);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PaidMediaPhoto) {
    auto result(std::make_shared<PaidMediaPhoto>());
    result->photo = parseRequiredArray<PhotoSize>(data, "photo");
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("photo", object->photo);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PaidMediaPreview) {
    auto result(std::make_shared<PaidMediaPreview>());
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaPreview) {
    JsonWrapper json;
    if (object) {
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PaidMediaPurchased) {
    auto result(std::make_shared<PaidMediaPurchased>());
    result->from = parseRequired<User>(data, "from");
    parse(data, "paid_media_payload", &result->paidMediaPayload);
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaPurchased) {
    JsonWrapper json;
    if (object) {
        json.put("from", object->from);
        json.put("paid_media_payload", object->paidMediaPayload);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PaidMediaVideo) {
    auto result(std::make_shared<PaidMediaVideo>());
    result->video = parseRequired<Video>(data, "video");
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaVideo) {
    JsonWrapper json;
    if (object) {
        json.put("video", object->video);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PaidMessagePriceChanged) {
    auto result(std::make_shared<PaidMessagePriceChanged>());
    parse(data, "paid_message_star_count", &result->paidMessageStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMessagePriceChanged) {
    JsonWrapper json;
    if (object) {
        json.put("paid_message_star_count", object->paidMessageStarCount);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(PreparedInlineMessage) {
    auto result(std::make_shared<PreparedInlineMessage>());
    parse(data, "id", &result->id);
    parse(data, "expiration_date", &result->expirationDate);
    return result;
}

DECLARE_PARSER_TO_JSON(PreparedInlineMessage) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("expiration_date", object->expirationDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(ReactionTypePaid) {
    auto result(std::make_shared<ReactionTypePaid>());
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionTypePaid) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(RefundedPayment) {
    auto result(std::make_shared<RefundedPayment>());
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "telegram_payment_charge_id", &result->telegramPaymentChargeId);
    parse(data, "provider_payment_charge_id", &result->providerPaymentChargeId);
    return result;
}

DECLARE_PARSER_TO_JSON(RefundedPayment) {
    JsonWrapper json;
    if (object) {
        json.put("currency", object->currency);
        json.put("total_amount", object->totalAmount);
        json.put("invoice_payload", object->invoicePayload);
        json.put("telegram_payment_charge_id", object->telegramPaymentChargeId);
        json.put("provider_payment_charge_id", object->providerPaymentChargeId);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(RevenueWithdrawalStateFailed) {
    auto result(std::make_shared<RevenueWithdrawalStateFailed>());
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(RevenueWithdrawalStateFailed) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(RevenueWithdrawalStatePending) {
    auto result(std::make_shared<RevenueWithdrawalStatePending>());
    return result;
}

DECLARE_PARSER_TO_JSON(RevenueWithdrawalStatePending) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(RevenueWithdrawalStateSucceeded) {
    auto result(std::make_shared<RevenueWithdrawalStateSucceeded>());
    parse(data, "date", &result->date);
    parse(data, "url", &result->url);
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(RevenueWithdrawalStateSucceeded) {
    JsonWrapper json;
    if (object) {
        json.put("date", object->date);
        json.put("url", object->url);
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(RevenueWithdrawalState) {
    if (data["type"] == RevenueWithdrawalStateFailed::TYPE) {
        return parse<RevenueWithdrawalStateFailed>(data);
    } else if (data["type"] == RevenueWithdrawalStatePending::TYPE) {
        return parse<RevenueWithdrawalStatePending>(data);
    } else if (data["type"] == RevenueWithdrawalStateSucceeded::TYPE) {
        return parse<RevenueWithdrawalStateSucceeded>(data);
    } else {
        throw invalidType("json", data.dump());
    }
}

DECLARE_PARSER_TO_JSON(RevenueWithdrawalState) {
    JsonWrapper json;
    if (object) {
        if (object->type == RevenueWithdrawalStateFailed::TYPE) {
            json = put<RevenueWithdrawalStateFailed>(object);
        } else if (object->type == RevenueWithdrawalStatePending::TYPE) {
            json = put<RevenueWithdrawalStatePending>(object);
        }
        else if (object->type == RevenueWithdrawalStateSucceeded::TYPE) {
            json = put<RevenueWithdrawalStateSucceeded>(object);
        }
        else {
            throw invalidType("type", object->type);
        }
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StarAmount) {
    auto result(std::make_shared<StarAmount>());
    parse(data, "amount", &result->amount);
    parse(data, "nanostar_amount", &result->nanostarAmount);
    return result;
}

DECLARE_PARSER_TO_JSON(StarAmount) {
    JsonWrapper json;
    if (object) {
        json.put("amount", object->amount);
        json.put("nanostar_amount", object->nanostarAmount);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StarTransaction) {
    auto result(std::make_shared<StarTransaction>());
    parse(data, "id", &result->id);
    parse(data, "date", &result->date);
    parse(data, "amount", &result->amount);
    parse(data, "nanostar_amount", &result->nanostarAmount);
    result->source = parseRequired<TransactionPartner>(data, "source");
    result->receiver = parseRequired<TransactionPartner>(data, "receiver");
    return result;
}

DECLARE_PARSER_TO_JSON(StarTransaction) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("date", object->date);
        json.put("amount", object->amount);
        json.put("nanostar_amount", object->nanostarAmount);
        json.put("source", object->source);
        json.put("receiver", object->receiver);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StarTransactions) {
    auto result(std::make_shared<StarTransactions>());
    result->transactions = parseRequiredArray<StarTransaction>(data, "transactions");
    return result;
}

DECLARE_PARSER_TO_JSON(StarTransactions) {
    JsonWrapper json;
    if (object) {
        json.put("transactions", object->transactions);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryArea) {
    auto result(std::make_shared<StoryArea>());
    result->position = parseRequired<StoryAreaPosition>(data, "position");
    result->type = parseRequired<StoryAreaType>(data, "type");
    return result;
}

DECLARE_PARSER_TO_JSON(StoryArea) {
    JsonWrapper json;
    if (object) {
        json.put("position", object->position);
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryAreaPosition) {
    auto result(std::make_shared<StoryAreaPosition>());
    parse(data, "x_percentage", &result->xPercentage);
    parse(data, "y_percentage", &result->yPercentage);
    parse(data, "width_percentage", &result->widthPercentage);
    parse(data, "height_percentage", &result->heightPercentage);
    parse(data, "rotation_angle", &result->rotationAngle);
    parse(data, "corner_radius_percentage", &result->cornerRadiusPercentage);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaPosition) {
    JsonWrapper json;
    if (object) {
        json.put("x_percentage", object->xPercentage);
        json.put("y_percentage", object->yPercentage);
        json.put("width_percentage", object->widthPercentage);
        json.put("height_percentage", object->heightPercentage);
        json.put("rotation_angle", object->rotationAngle);
        json.put("corner_radius_percentage", object->cornerRadiusPercentage);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryAreaType) {
    auto result(std::make_shared<StoryAreaType>());
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaType) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryAreaTypeLink) {
    auto result(std::make_shared<StoryAreaTypeLink>());
    parse(data, "url", &result->url);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeLink) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryAreaTypeLocation) {
    auto result(std::make_shared<StoryAreaTypeLocation>());
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "address", &result->address);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeLocation) {
    JsonWrapper json;
    if (object) {
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("address", object->address);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryAreaTypeSuggestedReaction) {
    auto result(std::make_shared<StoryAreaTypeSuggestedReaction>());
    result->reactionType = parseRequired<ReactionType>(data, "reaction_type");
    parse(data, "is_dark", &result->isDark);
    parse(data, "is_flipped", &result->isFlipped);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeSuggestedReaction) {
    JsonWrapper json;
    if (object) {
        json.put("reaction_type", object->reactionType);
        json.put("is_dark", object->isDark);
        json.put("is_flipped", object->isFlipped);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryAreaTypeUniqueGift) {
    auto result(std::make_shared<StoryAreaTypeUniqueGift>());
    parse(data, "name", &result->name);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeUniqueGift) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(StoryAreaTypeWeather) {
    auto result(std::make_shared<StoryAreaTypeWeather>());
    parse(data, "temperature", &result->temperature);
    parse(data, "emoji", &result->emoji);
    parse(data, "background_color", &result->backgroundColor);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeWeather) {
    JsonWrapper json;
    if (object) {
        json.put("temperature", object->temperature);
        json.put("emoji", object->emoji);
        json.put("background_color", object->backgroundColor);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostApprovalFailed) {
    auto result(std::make_shared<SuggestedPostApprovalFailed>());
    parse(data, "suggested_post_message", &result->suggestedPostMessage);
    result->price = parseRequired<SuggestedPostPrice>(data, "price");
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostApprovalFailed) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("price", object->price);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostApproved) {
    auto result(std::make_shared<SuggestedPostApproved>());
    parse(data, "suggested_post_message", &result->suggestedPostMessage);
    parse(data, "price", &result->price);
    parse(data, "send_date", &result->sendDate);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostApproved) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostDeclined) {
    auto result(std::make_shared<SuggestedPostDeclined>());
    parse(data, "suggested_post_message", &result->suggestedPostMessage);
    parse(data, "comment", &result->comment);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostDeclined) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("comment", object->comment);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostInfo) {
    auto result(std::make_shared<SuggestedPostInfo>());
    parse(data, "state", &result->state);
    parse(data, "price", &result->price);
    parse(data, "send_date", &result->sendDate);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostInfo) {
    JsonWrapper json;
    if (object) {
        json.put("state", object->state);
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostPaid) {
    auto result(std::make_shared<SuggestedPostPaid>());
    parse(data, "suggested_post_message", &result->suggestedPostMessage);
    parse(data, "currency", &result->currency);
    parse(data, "amount", &result->amount);
    parse(data, "star_amount", &result->starAmount);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostPaid) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("currency", object->currency);
        json.put("amount", object->amount);
        json.put("star_amount", object->starAmount);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostParameters) {
    auto result(std::make_shared<SuggestedPostParameters>());
    parse(data, "price", &result->price);
    parse(data, "send_date", &result->sendDate);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostParameters) {
    JsonWrapper json;
    if (object) {
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostPrice) {
    auto result(std::make_shared<SuggestedPostPrice>());
    parse(data, "currency", &result->currency);
    parse(data, "amount", &result->amount);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostPrice) {
    JsonWrapper json;
    if (object) {
        json.put("currency", object->currency);
        json.put("amount", object->amount);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(SuggestedPostRefunded) {
    auto result(std::make_shared<SuggestedPostRefunded>());
    parse(data, "suggested_post_message", &result->suggestedPostMessage);
    parse(data, "reason", &result->reason);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostRefunded) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("reason", object->reason);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartnerAffiliateProgram) {
    auto result(std::make_shared<TransactionPartnerAffiliateProgram>());
    parse(data, "sponsor_user", &result->sponsorUser);
    parse(data, "commission_per_mille", &result->commissionPerMille);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerAffiliateProgram) {
    JsonWrapper json;
    if (object) {
        json.put("sponsor_user", object->sponsorUser);
        json.put("commission_per_mille", object->commissionPerMille);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartnerChat) {
    auto result(std::make_shared<TransactionPartnerChat>());
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "gift", &result->gift);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerChat) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("gift", object->gift);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartnerFragment) {
    auto result(std::make_shared<TransactionPartnerFragment>());
    parse(data, "withdrawal_state", &result->withdrawalState);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerFragment) {
    JsonWrapper json;
    if (object) {
        json.put("withdrawal_state", object->withdrawalState);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartnerOther) {
    auto result(std::make_shared<TransactionPartnerOther>());
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerOther) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartnerTelegramAds) {
    auto result(std::make_shared<TransactionPartnerTelegramAds>());
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerTelegramAds) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartnerTelegramApi) {
    auto result(std::make_shared<TransactionPartnerTelegramApi>());
    parse(data, "request_count", &result->requestCount);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerTelegramApi) {
    JsonWrapper json;
    if (object) {
        json.put("request_count", object->requestCount);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartnerUser) {
    auto result(std::make_shared<TransactionPartnerUser>());
    parse(data, "transaction_type", &result->transactionType);
    result->user = parseRequired<User>(data, "user");
    parse(data, "affiliate", &result->affiliate);
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "subscription_period", &result->subscriptionPeriod);
    result->paidMedia = parseArray<PaidMedia>(data, "paid_media");
    parse(data, "paid_media_payload", &result->paidMediaPayload);
    parse(data, "gift", &result->gift);
    parse(data, "premium_subscription_duration", &result->premiumSubscriptionDuration);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerUser) {
    JsonWrapper json;
    if (object) {
        json.put("transaction_type", object->transactionType);
        json.put("user", object->user);
        json.put("affiliate", object->affiliate);
        json.put("invoice_payload", object->invoicePayload);
        json.put("subscription_period", object->subscriptionPeriod);
        json.put("paid_media", object->paidMedia);
        json.put("paid_media_payload", object->paidMediaPayload);
        json.put("gift", object->gift);
        json.put("premium_subscription_duration", object->premiumSubscriptionDuration);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(TransactionPartner) {
    if (data["type"] == TransactionPartnerAffiliateProgram::TYPE) {
        return parse<TransactionPartnerAffiliateProgram>(data);
    } else if (data["type"] == TransactionPartnerChat::TYPE) {
        return parse<TransactionPartnerChat>(data);
    } else if (data["type"] == TransactionPartnerFragment::TYPE) {
        return parse<TransactionPartnerFragment>(data);
    } else if (data["type"] == TransactionPartnerOther::TYPE) {
        return parse<TransactionPartnerOther>(data);
    } else if (data["type"] == TransactionPartnerTelegramAds::TYPE) {
        return parse<TransactionPartnerTelegramAds>(data);
    } else if (data["type"] == TransactionPartnerTelegramApi::TYPE) {
        return parse<TransactionPartnerTelegramApi>(data);
    } else if (data["type"] == TransactionPartnerUser::TYPE) {
        return parse<TransactionPartnerUser>(data);
    } else {
        throw invalidType("json", data.dump());
    }
}

DECLARE_PARSER_TO_JSON(TransactionPartner) {
    JsonWrapper json;
    if (object) {
        if (object->type == TransactionPartnerAffiliateProgram::TYPE) {
            json = put<TransactionPartnerAffiliateProgram>(object);
        } else if (object->type == TransactionPartnerChat::TYPE) {
            json = put<TransactionPartnerChat>(object);
        } else if (object->type == TransactionPartnerFragment::TYPE) {
            json = put<TransactionPartnerFragment>(object);
        } else if (object->type == TransactionPartnerOther::TYPE) {
            json = put<TransactionPartnerOther>(object);
        } else if (object->type == TransactionPartnerTelegramAds::TYPE) {
            json = put<TransactionPartnerTelegramAds>(object);
        } else if (object->type == TransactionPartnerTelegramApi::TYPE) {
            json = put<TransactionPartnerTelegramApi>(object);
        } else if (object->type == TransactionPartnerUser::TYPE) {
            json = put<TransactionPartnerUser>(object);
        } else {
            throw invalidType("type", object->type);
        }
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UniqueGift) {
    auto result(std::make_shared<UniqueGift>());
    parse(data, "gift_id", &result->giftId);
    parse(data, "base_name", &result->baseName);
    parse(data, "name", &result->name);
    parse(data, "number", &result->number);
    result->model = parseRequired<UniqueGiftModel>(data, "model");
    result->symbol = parseRequired<UniqueGiftSymbol>(data, "symbol");
    result->backdrop = parseRequired<UniqueGiftBackdrop>(data, "backdrop");
    parse(data, "is_premium", &result->isPremium);
    parse(data, "is_burned", &result->isBurned);
    parse(data, "is_from_blockchain", &result->isFromBlockchain);
    parse(data, "colors", &result->colors);
    parse(data, "publisher_chat", &result->publisherChat);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGift) {
    JsonWrapper json;
    if (object) {
        json.put("gift_id", object->giftId);
        json.put("base_name", object->baseName);
        json.put("name", object->name);
        json.put("number", object->number);
        json.put("model", object->model);
        json.put("symbol", object->symbol);
        json.put("backdrop", object->backdrop);
        json.put("is_premium", object->isPremium);
        json.put("is_burned", object->isBurned);
        json.put("is_from_blockchain", object->isFromBlockchain);
        json.put("colors", object->colors);
        json.put("publisher_chat", object->publisherChat);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UniqueGiftBackdrop) {
    auto result(std::make_shared<UniqueGiftBackdrop>());
    parse(data, "name", &result->name);
    result->colors = parseRequired<UniqueGiftBackdropColors>(data, "colors");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftBackdrop) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("colors", object->colors);
        json.put("rarity_per_mille", object->rarityPerMille);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UniqueGiftBackdropColors) {
    auto result(std::make_shared<UniqueGiftBackdropColors>());
    parse(data, "center_color", &result->centerColor);
    parse(data, "edge_color", &result->edgeColor);
    parse(data, "symbol_color", &result->symbolColor);
    parse(data, "text_color", &result->textColor);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftBackdropColors) {
    JsonWrapper json;
    if (object) {
        json.put("center_color", object->centerColor);
        json.put("edge_color", object->edgeColor);
        json.put("symbol_color", object->symbolColor);
        json.put("text_color", object->textColor);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UniqueGiftColors) {
    auto result(std::make_shared<UniqueGiftColors>());
    parse(data, "model_custom_emoji_id", &result->modelCustomEmojiId);
    parse(data, "symbol_custom_emoji_id", &result->symbolCustomEmojiId);
    parse(data, "light_theme_main_color", &result->lightThemeMainColor);
    result->lightThemeOtherColors = parsePrimitiveRequiredArray<int64_t>(data, "light_theme_other_colors");
    parse(data, "dark_theme_main_color", &result->darkThemeMainColor);
    result->darkThemeOtherColors = parsePrimitiveRequiredArray<int64_t>(data, "dark_theme_other_colors");
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftColors) {
    JsonWrapper json;
    if (object) {
        json.put("model_custom_emoji_id", object->modelCustomEmojiId);
        json.put("symbol_custom_emoji_id", object->symbolCustomEmojiId);
        json.put("light_theme_main_color", object->lightThemeMainColor);
        json.put("light_theme_other_colors", object->lightThemeOtherColors);
        json.put("dark_theme_main_color", object->darkThemeMainColor);
        json.put("dark_theme_other_colors", object->darkThemeOtherColors);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UniqueGiftInfo) {
    auto result(std::make_shared<UniqueGiftInfo>());
    result->gift = parseRequired<UniqueGift>(data, "gift");
    parse(data, "origin", &result->origin);
    parse(data, "last_resale_currency", &result->lastResaleCurrency);
    parse(data, "last_resale_amount", &result->lastResaleAmount);
    parse(data, "owned_gift_id", &result->ownedGiftId);
    parse(data, "transfer_star_count", &result->transferStarCount);
    parse(data, "next_transfer_date", &result->nextTransferDate);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftInfo) {
    JsonWrapper json;
    if (object) {
        json.put("gift", object->gift);
        json.put("origin", object->origin);
        json.put("last_resale_currency", object->lastResaleCurrency);
        json.put("last_resale_amount", object->lastResaleAmount);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("transfer_star_count", object->transferStarCount);
        json.put("next_transfer_date", object->nextTransferDate);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UniqueGiftModel) {
    auto result(std::make_shared<UniqueGiftModel>());
    parse(data, "name", &result->name);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    parse(data, "rarity", &result->rarity);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftModel) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("sticker", object->sticker);
        json.put("rarity_per_mille", object->rarityPerMille);
        json.put("rarity", object->rarity);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UniqueGiftSymbol) {
    auto result(std::make_shared<UniqueGiftSymbol>());
    parse(data, "name", &result->name);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftSymbol) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("sticker", object->sticker);
        json.put("rarity_per_mille", object->rarityPerMille);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UserProfileAudios) {
    auto result(std::make_shared<UserProfileAudios>());
    parse(data, "total_count", &result->totalCount);
    result->audios = parseRequiredArray<Audio>(data, "audios");
    return result;
}

DECLARE_PARSER_TO_JSON(UserProfileAudios) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("audios", object->audios);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(UserRating) {
    auto result(std::make_shared<UserRating>());
    parse(data, "level", &result->level);
    parse(data, "rating", &result->rating);
    parse(data, "current_level_rating", &result->currentLevelRating);
    parse(data, "next_level_rating", &result->nextLevelRating);
    return result;
}

DECLARE_PARSER_TO_JSON(UserRating) {
    JsonWrapper json;
    if (object) {
        json.put("level", object->level);
        json.put("rating", object->rating);
        json.put("current_level_rating", object->currentLevelRating);
        json.put("next_level_rating", object->nextLevelRating);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(VideoQuality) {
    auto result(std::make_shared<VideoQuality>());
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "codec", &result->codec);
    parse(data, "file_size", &result->fileSize);
    return result;
}

DECLARE_PARSER_TO_JSON(VideoQuality) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("codec", object->codec);
        json.put("file_size", object->fileSize);
    }
    return json;
}

DECLARE_PARSER_FROM_JSON(AffiliateInfo) {
    auto result(std::make_shared<AffiliateInfo>());
    result->affiliateUser = parseRequired<User>(data, "affiliate_user");
    result->affiliateChat = parseRequired<Chat>(data, "affiliate_chat");
    parse(data, "commission_per_mille", &result->commissionPerMille);
    parse(data, "amount", &result->amount);
    parse(data, "nanostar_amount", &result->nanostarAmount);
    return result;
}

DECLARE_PARSER_TO_JSON(AffiliateInfo) {
    JsonWrapper json;
    if (object) {
        json.put("affiliate_user", object->affiliateUser);
        json.put("affiliate_chat", object->affiliateChat);
        json.put("commission_per_mille", object->commissionPerMille);
        json.put("amount", object->amount);
        json.put("nanostar_amount", object->nanostarAmount);
    }
    return json;
}

}  // namespace TgBot
