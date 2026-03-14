#ifndef TGBOT_MESSAGE_H
#define TGBOT_MESSAGE_H

#include "tgbot/types/DirectMessagesTopic.h"
#include "tgbot/types/ChatOwnerLeft.h"
#include "tgbot/types/SuggestedPostApproved.h"
#include "tgbot/types/SuggestedPostApprovalFailed.h"
#include "tgbot/types/ChecklistTasksAdded.h"
#include "tgbot/types/RefundedPayment.h"
#include "tgbot/types/DirectMessagePriceChanged.h"
#include "tgbot/types/SuggestedPostInfo.h"
#include "tgbot/types/PaidMessagePriceChanged.h"
#include "tgbot/types/ChatBackground.h"
#include "tgbot/types/UniqueGiftInfo.h"
#include "tgbot/types/Checklist.h"
#include "tgbot/types/SuggestedPostPaid.h"
#include "tgbot/types/PaidMediaInfo.h"
#include "tgbot/types/SuggestedPostRefunded.h"
#include "tgbot/types/ChecklistTasksDone.h"
#include "tgbot/types/ChatOwnerChanged.h"
#include "tgbot/types/SuggestedPostDeclined.h"
#include "tgbot/types/GiftInfo.h"

#include "tgbot/types/User.h"
#include "tgbot/types/Chat.h"
#include "tgbot/types/MessageOrigin.h"
#include "tgbot/types/ExternalReplyInfo.h"
#include "tgbot/types/TextQuote.h"
#include "tgbot/types/Story.h"
#include "tgbot/types/MessageEntity.h"
#include "tgbot/types/LinkPreviewOptions.h"
#include "tgbot/types/Animation.h"
#include "tgbot/types/Audio.h"
#include "tgbot/types/Document.h"
#include "tgbot/types/PhotoSize.h"
#include "tgbot/types/Sticker.h"
#include "tgbot/types/Video.h"
#include "tgbot/types/VideoNote.h"
#include "tgbot/types/Voice.h"
#include "tgbot/types/Contact.h"
#include "tgbot/types/Dice.h"
#include "tgbot/types/Game.h"
#include "tgbot/types/Poll.h"
#include "tgbot/types/Venue.h"
#include "tgbot/types/Location.h"
#include "tgbot/types/MessageAutoDeleteTimerChanged.h"
#include "tgbot/types/Invoice.h"
#include "tgbot/types/SuccessfulPayment.h"
#include "tgbot/types/UsersShared.h"
#include "tgbot/types/ChatShared.h"
#include "tgbot/types/WriteAccessAllowed.h"
#include "tgbot/types/PassportData.h"
#include "tgbot/types/ProximityAlertTriggered.h"
#include "tgbot/types/ChatBoostAdded.h"
#include "tgbot/types/ForumTopicCreated.h"
#include "tgbot/types/ForumTopicEdited.h"
#include "tgbot/types/ForumTopicClosed.h"
#include "tgbot/types/ForumTopicReopened.h"
#include "tgbot/types/GeneralForumTopicHidden.h"
#include "tgbot/types/GeneralForumTopicUnhidden.h"
#include "tgbot/types/GiveawayCreated.h"
#include "tgbot/types/Giveaway.h"
#include "tgbot/types/GiveawayWinners.h"
#include "tgbot/types/GiveawayCompleted.h"
#include "tgbot/types/VideoChatScheduled.h"
#include "tgbot/types/VideoChatStarted.h"
#include "tgbot/types/VideoChatEnded.h"
#include "tgbot/types/VideoChatParticipantsInvited.h"
#include "tgbot/types/WebAppData.h"
#include "tgbot/types/InlineKeyboardMarkup.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object represents a message.
 *
 * @ingroup types
 */
class Message {

public:
    using Ptr = std::shared_ptr<Message>;

    /**
     * @brief Unique message identifier inside this chat
     */
    std::int32_t messageId;

    /**
     * @brief Optional. Unique identifier of a message thread to which the message belongs; for supergroups only
     */
    std::optional<std::int32_t> messageThreadId;

    /**
     * @brief Optional. Sender of the message; empty for messages sent to channels.
     *
     * For backward compatibility, the field contains a fake sender user in non-channel chats, if the message was sent on behalf of a chat.
     */
    std::optional<User::Ptr> from;

    /**
     * @brief Optional. Sender of the message, sent on behalf of a chat.
     *
     * For example, the channel itself for channel posts, the supergroup itself for messages from anonymous group administrators, the linked channel for messages automatically forwarded to the discussion group.
     * For backward compatibility, the field from contains a fake sender user in non-channel chats, if the message was sent on behalf of a chat.
     */
    std::optional<Chat::Ptr> senderChat;

    /**
     * @brief Optional. If the sender of the message boosted the chat, the number of boosts added by the user
     */
    std::optional<std::int32_t> senderBoostCount;

    /**
     * @brief Optional. The bot that actually sent the message on behalf of the business account.
     *
     * Available only for outgoing messages sent on behalf of the connected business account.
     */
    std::optional<User::Ptr> senderBusinessBot;

    /**
     * @brief Date the message was sent in Unix time.
     *
     * It is always a positive number, representing a valid date.
     */
    std::uint32_t date;

    /**
     * @brief Optional. Unique identifier of the business connection from which the message was received.
     *
     * If non-empty, the message belongs to a chat of the corresponding business account that is independent from any potential bot chat which might share the same identifier.
     */
    std::optional<std::string> businessConnectionId;

    /**
     * @brief Chat the message belongs to
     */
    Chat::Ptr chat;

    /**
     * @brief Optional. Information about the original message for forwarded messages
     */
    std::optional<MessageOrigin::Ptr> forwardOrigin;

    /**
     * @brief Optional. True, if the message is sent to a forum topic
     */
    std::optional<bool> isTopicMessage;

    /**
     * @brief Optional. True, if the message is a channel post that was automatically forwarded to the connected discussion group
     */
    std::optional<bool> isAutomaticForward;

    /**
     * @brief Optional. For replies in the same chat and message thread, the original message.
     *
     * Note that the Message object in this field will not contain further replyToMessage fields even if it itself is a reply.
     */
    std::optional<Message::Ptr> replyToMessage;

    /**
     * @brief Optional. Information about the message that is being replied to, which may come from another chat or forum topic
     */
    std::optional<ExternalReplyInfo::Ptr> externalReply;

    /**
     * @brief Optional. For replies that quote part of the original message, the quoted part of the message
     */
    std::optional<TextQuote::Ptr> quote;

    /**
     * @brief Optional. For replies to a story, the original story
     */
    std::optional<Story::Ptr> replyToStory;

    /**
     * @brief Optional. Bot through which the message was sent
     */
    std::optional<User::Ptr> viaBot;

    /**
     * @brief Optional. Date the message was last edited in Unix time
     */
    std::optional<std::uint32_t> editDate;

    /**
     * @brief Optional. True, if the message can't be forwarded
     */
    std::optional<bool> hasProtectedContent;

    /**
     * @brief Optional. True, if the message was sent by an implicit action, for example, as an away or a greeting business message, or as a scheduled message
     */
    std::optional<bool> isFromOffline;

    /**
     * @brief Optional. The unique identifier of a media message group this message belongs to
     */
    std::optional<std::string> mediaGroupId;

    /**
     * @brief Optional. Signature of the post author for messages in channels, or the custom title of an anonymous group administrator
     */
    std::optional<std::string> authorSignature;

    /**
     * @brief Optional. For text messages, the actual UTF-8 text of the message
     */
    std::optional<std::string> text;

    /**
     * @brief Optional. For text messages, special entities like usernames, URLs, bot commands, etc. that appear in the text
     */
    std::optional<std::vector<MessageEntity::Ptr>> entities;

    /**
     * @brief Optional. Options used for link preview generation for the message, if it is a text message and link preview options were changed
     */
    std::optional<LinkPreviewOptions::Ptr> linkPreviewOptions;

    /**
     * @brief Optional. Message is an animation, information about the animation.
     *
     * For backward compatibility, when this field is set, the document field will also be set
     */
    std::optional<Animation::Ptr> animation;

    /**
     * @brief Optional. Message is an audio file, information about the file
     */
    std::optional<Audio::Ptr> audio;

    /**
     * @brief Optional. Message is a general file, information about the file
     */
    std::optional<Document::Ptr> document;

    /**
     * @brief Optional. Message is a photo, available sizes of the photo
     */
    std::optional<std::vector<PhotoSize::Ptr>> photo;

    /**
     * @brief Optional. Message is a sticker, information about the sticker
     */
    std::optional<Sticker::Ptr> sticker;

    /**
     * @brief Optional. Message is a forwarded story
     */
    std::optional<Story::Ptr> story;

    /**
     * @brief Optional. Message is a video, information about the video
     */
    std::optional<Video::Ptr> video;

    /**
     * @brief Optional. Message is a [video note](https://telegram.org/blog/video-messages-and-telescope), information about the video message
     */
    std::optional<VideoNote::Ptr> videoNote;

    /**
     * @brief Optional. Message is a voice message, information about the file
     */
    std::optional<Voice::Ptr> voice;

    /**
     * @brief Optional. Caption for the animation, audio, document, photo, video or voice
     */
    std::optional<std::string> caption;

    /**
     * @brief Optional. For messages with a caption, special entities like usernames, URLs, bot commands, etc. that appear in the caption
     */
    std::optional<std::vector<MessageEntity::Ptr>> captionEntities;

    /**
     * @brief Optional. True, if the message media is covered by a spoiler animation
     */
    std::optional<bool> hasMediaSpoiler;

    /**
     * @brief Optional. Message is a shared contact, information about the contact
     */
    std::optional<Contact::Ptr> contact;

    /**
     * @brief Optional. Message is a dice with random value
     */
    std::optional<Dice::Ptr> dice;

    /**
     * @brief Optional. Message is a game, information about the game.
     *
     * [More about games »](https://core.telegram.org/bots/api#games)
     */
    std::optional<Game::Ptr> game;

    /**
     * @brief Optional. Message is a native poll, information about the poll
     */
    std::optional<Poll::Ptr> poll;

    /**
     * @brief Optional. Message is a venue, information about the venue.
     *
     * For backward compatibility, when this field is set, the location field will also be set
     */
    std::optional<Venue::Ptr> venue;

    /**
     * @brief Optional. Message is a shared location, information about the location
     */
    std::optional<Location::Ptr> location;

    /**
     * @brief Optional. New members that were added to the group or supergroup and information about them (the bot itself may be one of these members)
     */
    std::optional<std::vector<User::Ptr>> newChatMembers;

    /**
     * @brief Optional. A member was removed from the group, information about them (this member may be the bot itself)
     */
    std::optional<User::Ptr> leftChatMember;

    /**
     * @brief Optional. A chat title was changed to this value
     */
    std::optional<std::string> newChatTitle;

    /**
     * @brief Optional. A chat photo was change to this value
     */
    std::optional<std::vector<PhotoSize::Ptr>> newChatPhoto;

    /**
     * @brief Optional. Service message: the chat photo was deleted
     */
    std::optional<bool> deleteChatPhoto;

    /**
     * @brief Optional. Service message: the group has been created
     */
    std::optional<bool> groupChatCreated;

    /**
     * @brief Optional. Service message: the supergroup has been created.
     *
     * This field can't be received in a message coming through updates, because bot can't be a member of a supergroup when it is created.
     * It can only be found in replyToMessage if someone replies to a very first message in a directly created supergroup.
     */
    std::optional<bool> supergroupChatCreated;

    /**
     * @brief Optional. Service message: the channel has been created.
     *
     * This field can't be received in a message coming through updates, because bot can't be a member of a channel when it is created.
     * It can only be found in replyToMessage if someone replies to a very first message in a channel.
     */
    std::optional<bool> channelChatCreated;

    /**
     * @brief Optional. Service message: auto-delete timer settings changed in the chat
     */
    std::optional<MessageAutoDeleteTimerChanged::Ptr> messageAutoDeleteTimerChanged;

    /**
     * @brief Optional. The group has been migrated to a supergroup with the specified identifier.
     *
     * This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it.
     * But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
     */
    std::optional<std::int64_t> migrateToChatId;

    /**
     * @brief Optional. The supergroup has been migrated from a group with the specified identifier.
     *
     * This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it.
     * But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
     */
    std::optional<std::int64_t> migrateFromChatId;

    /**
     * @brief Optional. Specified message was pinned.
     *
     * Note that the Message object in this field will not contain further replyToMessage fields even if it itself is a reply.
     */
    std::optional<Message::Ptr> pinnedMessage;

    /**
     * @brief Optional. Message is an invoice for a [payment](https://core.telegram.org/bots/api#payments), information about the invoice.
     *
     * [More about payments »](https://core.telegram.org/bots/api#payments)
     */
    std::optional<Invoice::Ptr> invoice;

    /**
     * @brief Optional. Message is a service message about a successful payment, information about the payment.
     *
     * [More about payments »](https://core.telegram.org/bots/api#payments)
     */
    std::optional<SuccessfulPayment::Ptr> successfulPayment;

    /**
     * @brief Optional. Service message: users were shared with the bot
     */
    std::optional<UsersShared::Ptr> usersShared;

    /**
     * @brief Optional. Service message: a chat was shared with the bot
     */
    std::optional<ChatShared::Ptr> chatShared;

    /**
     * @brief Optional. The domain name of the website on which the user has logged in.
     *
     * [More about Telegram Login »](https://core.telegram.org/widgets/login)
     */
    std::optional<std::string> connectedWebsite;

    /**
     * @brief Optional. Service message: the user allowed the bot to write messages after adding it to the attachment or side menu, launching a Web App from a link, or accepting an explicit request from a Web App sent by the method [requestWriteAccess](https://core.telegram.org/bots/webapps#initializing-mini-apps)
     */
    std::optional<WriteAccessAllowed::Ptr> writeAccessAllowed;

    /**
     * @brief Optional. Telegram Passport data
     */
    std::optional<PassportData::Ptr> passportData;

    /**
     * @brief Optional. Service message.
     *
     * A user in the chat triggered another user's proximity alert while sharing Live Location.
     */
    std::optional<ProximityAlertTriggered::Ptr> proximityAlertTriggered;

    /**
     * @brief Optional. Service message: user boosted the chat
     */
    std::optional<ChatBoostAdded::Ptr> boostAdded;

    /**
     * @brief Optional. Service message: forum topic created
     */
    std::optional<ForumTopicCreated::Ptr> forumTopicCreated;

    /**
     * @brief Optional. Service message: forum topic edited
     */
    std::optional<ForumTopicEdited::Ptr> forumTopicEdited;

    /**
     * @brief Optional. Service message: forum topic closed
     */
    std::optional<ForumTopicClosed::Ptr> forumTopicClosed;

    /**
     * @brief Optional. Service message: forum topic reopened
     */
    std::optional<ForumTopicReopened::Ptr> forumTopicReopened;

    /**
     * @brief Optional. Service message: the 'General' forum topic hidden
     */
    std::optional<GeneralForumTopicHidden::Ptr> generalForumTopicHidden;

    /**
     * @brief Optional. Service message: the 'General' forum topic unhidden
     */
    std::optional<GeneralForumTopicUnhidden::Ptr> generalForumTopicUnhidden;

    /**
     * @brief Optional. Service message: a scheduled giveaway was created
     */
    std::optional<GiveawayCreated::Ptr> giveawayCreated;

    /**
     * @brief Optional. The message is a scheduled giveaway message
     */
    std::optional<Giveaway::Ptr> giveaway;

    /**
     * @brief Optional. A giveaway with public winners was completed
     */
    std::optional<GiveawayWinners::Ptr> giveawayWinners;

    /**
     * @brief Optional. Service message: a giveaway without public winners was completed
     */
    std::optional<GiveawayCompleted::Ptr> giveawayCompleted;

    /**
     * @brief Optional. Service message: video chat scheduled
     */
    std::optional<VideoChatScheduled::Ptr> videoChatScheduled;

    /**
     * @brief Optional. Service message: video chat started
     */
    std::optional<VideoChatStarted::Ptr> videoChatStarted;

    /**
     * @brief Optional. Service message: video chat ended
     */
    std::optional<VideoChatEnded::Ptr> videoChatEnded;

    /**
     * @brief Optional. Service message: new participants invited to a video chat
     */
    std::optional<VideoChatParticipantsInvited::Ptr> videoChatParticipantsInvited;

    /**
     * @brief Optional. Service message: data sent by a Web App
     */
    std::optional<WebAppData::Ptr> webAppData;

    /**
     * @brief Optional. Inline keyboard attached to the message.
     *
     * loginUrl buttons are represented as ordinary url buttons.
     */
    std::optional<InlineKeyboardMarkup::Ptr> replyMarkup;

    /**
     * @brief Optional. Information about the direct messages chat topic that contains the message
     */
    std::optional<DirectMessagesTopic::Ptr> directMessagesTopic;

    /**
     * @brief Optional. Tag or custom title of the sender of the message; for supergroups only
     */
    std::optional<std::string> senderTag;

    /**
     * @brief Optional. Identifier of the specific checklist task that is being replied to
     */
    std::optional<std::int32_t> replyToChecklistTaskId;

    /**
     * @brief Optional. True, if the message is a paid post. Note that such posts must not be deleted for 24 hours to receive the payment and can't be edited.
     */
    std::optional<bool> isPaidPost;

    /**
     * @brief Optional. The number of Telegram Stars that were paid by the sender of the message to send it
     */
    std::optional<std::int32_t> paidStarCount;

    /**
     * @brief Optional. Information about suggested post parameters if the message is a suggested post in a channel direct messages chat. If the message is an approved or declined suggested post, then it can't be edited.
     */
    std::optional<SuggestedPostInfo::Ptr> suggestedPostInfo;

    /**
     * @brief Optional. Unique identifier of the message effect added to the message
     */
    std::optional<std::string> effectId;

    /**
     * @brief Optional. Message contains paid media; information about the paid media
     */
    std::optional<PaidMediaInfo::Ptr> paidMedia;

    /**
     * @brief Optional. True, if the caption must be shown above the message media
     */
    std::optional<bool> showCaptionAboveMedia;

    /**
     * @brief Optional. Message is a checklist
     */
    std::optional<Checklist::Ptr> checklist;

    /**
     * @brief Optional. Service message: chat owner has left
     */
    std::optional<ChatOwnerLeft::Ptr> chatOwnerLeft;

    /**
     * @brief Optional. Service message: chat owner has changed
     */
    std::optional<ChatOwnerChanged::Ptr> chatOwnerChanged;

    /**
     * @brief Optional. Message is a service message about a refunded payment, information about the payment. More about payments: https://core.telegram.org/bots/api#payments
     */
    std::optional<RefundedPayment::Ptr> refundedPayment;

    /**
     * @brief Optional. Service message: a regular gift was sent or received
     */
    std::optional<GiftInfo::Ptr> gift;

    /**
     * @brief Optional. Service message: a unique gift was sent or received
     */
    std::optional<UniqueGiftInfo::Ptr> uniqueGift;

    /**
     * @brief Optional. Service message: upgrade of a gift was purchased after the gift was sent
     */
    std::optional<GiftInfo::Ptr> giftUpgradeSent;

    /**
     * @brief Optional. Service message: chat background set
     */
    std::optional<ChatBackground::Ptr> chatBackgroundSet;

    /**
     * @brief Optional. Service message: some tasks in a checklist were marked as done or not done
     */
    std::optional<ChecklistTasksDone::Ptr> checklistTasksDone;

    /**
     * @brief Optional. Service message: tasks were added to a checklist
     */
    std::optional<ChecklistTasksAdded::Ptr> checklistTasksAdded;

    /**
     * @brief Optional. Service message: the price for paid messages in the corresponding direct messages chat of a channel has changed
     */
    std::optional<DirectMessagePriceChanged::Ptr> directMessagePriceChanged;

    /**
     * @brief Optional. Service message: the price for paid messages has changed in the chat
     */
    std::optional<PaidMessagePriceChanged::Ptr> paidMessagePriceChanged;

    /**
     * @brief Optional. Service message: a suggested post was approved
     */
    std::optional<SuggestedPostApproved::Ptr> suggestedPostApproved;

    /**
     * @brief Optional. Service message: approval of a suggested post has failed
     */
    std::optional<SuggestedPostApprovalFailed::Ptr> suggestedPostApprovalFailed;

    /**
     * @brief Optional. Service message: a suggested post was declined
     */
    std::optional<SuggestedPostDeclined::Ptr> suggestedPostDeclined;

    /**
     * @brief Optional. Service message: payment for a suggested post was received
     */
    std::optional<SuggestedPostPaid::Ptr> suggestedPostPaid;

    /**
     * @brief Optional. Service message: payment for a suggested post was refunded
     */
    std::optional<SuggestedPostRefunded::Ptr> suggestedPostRefunded;
};
}

#endif //TGBOT_MESSAGE_H
