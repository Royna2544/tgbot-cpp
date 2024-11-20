#ifndef TGBOT_UPDATE_H
#define TGBOT_UPDATE_H

#include <cstdint>
#include <memory>

#include "tgbot/types/BusinessConnection.h"
#include "tgbot/types/BusinessMessagesDeleted.h"
#include "tgbot/types/CallbackQuery.h"
#include "tgbot/types/ChatBoostRemoved.h"
#include "tgbot/types/ChatBoostUpdated.h"
#include "tgbot/types/ChatJoinRequest.h"
#include "tgbot/types/ChatMemberUpdated.h"
#include "tgbot/types/ChosenInlineResult.h"
#include "tgbot/types/InlineQuery.h"
#include "tgbot/types/Message.h"
#include "tgbot/types/MessageReactionCountUpdated.h"
#include "tgbot/types/MessageReactionUpdated.h"
#include "tgbot/types/Poll.h"
#include "tgbot/types/PollAnswer.h"
#include "tgbot/types/PreCheckoutQuery.h"
#include "tgbot/types/ShippingQuery.h"

namespace TgBot {

/**
 * @brief This [object](https://core.telegram.org/bots/api#available-types)
 * represents an incoming update.
 *
 * At most one of the optional parameters can be present in any given update.
 *
 * @ingroup types
 */
class Update {
   public:
    using Ptr = std::shared_ptr<Update>;

    /**
     * @brief The update's unique identifier.
     *
     * Update identifiers start from a certain positive number and increase
     * sequentially. This identifier becomes especially handy if you're using
     * [webhooks](https://core.telegram.org/bots/api#setwebhook), since it
     * allows you to ignore repeated updates or to restore the correct update
     * sequence, should they get out of order. If there are no new updates for
     * at least a week, then identifier of the next update will be chosen
     * randomly instead of sequentially.
     */
    std::int32_t updateId;

    /**
     * @brief Optional. New incoming message of any kind - text, photo, sticker,
     * etc.
     */
    Message::Ptr message;

    /**
     * @brief Optional. New version of a message that is known to the bot and
     * was edited.
     *
     * This update may at times be triggered by changes to message fields that
     * are either unavailable or not actively used by your bot.
     */
    Message::Ptr editedMessage;

    /**
     * @brief Optional. New incoming channel post of any kind - text, photo,
     * sticker, etc.
     */
    Message::Ptr channelPost;

    /**
     * @brief Optional. New version of a channel post that is known to the bot
     * and was edited.
     *
     * This update may at times be triggered by changes to message fields that
     * are either unavailable or not actively used by your bot.
     */
    Message::Ptr editedChannelPost;

    /**
     * @brief Optional. The bot was connected to or disconnected from a business
     * account, or a user edited an existing connection with the bot
     */
    BusinessConnection::Ptr businessConnection;

    /**
     * @brief Optional. New non-service message from a connected business
     * account
     */
    Message::Ptr businessMessage;

    /**
     * @brief Optional. New version of a message from a connected business
     * account
     */
    Message::Ptr editedBusinessMessage;

    /**
     * @brief Optional. Messages were deleted from a connected business account
     */
    BusinessMessagesDeleted::Ptr deletedBusinessMessages;

    /**
     * @brief Optional. A reaction to a message was changed by a user.
     *
     * The bot must be an administrator in the chat and must explicitly specify
     * "message_reaction" in the list of allowedUpdates to receive these
     * updates. The update isn't received for reactions set by bots.
     */
    MessageReactionUpdated::Ptr messageReaction;

    /**
     * @brief Optional. Reactions to a message with anonymous reactions were
     * changed.
     *
     * The bot must be an administrator in the chat and must explicitly specify
     * "message_reaction_count" in the list of allowedUpdates to receive these
     * updates. The updates are grouped and can be sent with delay up to a few
     * minutes.
     */
    MessageReactionCountUpdated::Ptr messageReactionCount;

    /**
     * @brief Optional. New incoming
     * [inline](https://core.telegram.org/bots/api#inline-mode) query
     */
    InlineQuery::Ptr inlineQuery;

    /**
     * @brief Optional. The result of an
     * [inline](https://core.telegram.org/bots/api#inline-mode) query that was
     * chosen by a user and sent to their chat partner.
     *
     * Please see our documentation on the [feedback
     * collecting](https://core.telegram.org/bots/inline#collecting-feedback)
     * for details on how to enable these updates for your bot.
     */
    ChosenInlineResult::Ptr chosenInlineResult;

    /**
     * @brief Optional. New incoming callback query
     */
    CallbackQuery::Ptr callbackQuery;

    /**
     * @brief Optional. New incoming shipping query.
     *
     * Only for invoices with flexible price
     */
    ShippingQuery::Ptr shippingQuery;

    /**
     * @brief Optional. New incoming pre-checkout query.
     *
     * Contains full information about checkout
     */
    PreCheckoutQuery::Ptr preCheckoutQuery;

    /**
     * @brief Optional. New poll state.
     *
     * Bots receive only updates about manually stopped polls and polls, which
     * are sent by the bot
     */
    Poll::Ptr poll;

    /**
     * @brief Optional. A user changed their answer in a non-anonymous poll.
     *
     * Bots receive new votes only in polls that were sent by the bot itself.
     */
    PollAnswer::Ptr pollAnswer;

    /**
     * @brief Optional. The bot's chat member status was updated in a chat.
     *
     * For private chats, this update is received only when the bot is blocked
     * or unblocked by the user.
     */
    ChatMemberUpdated::Ptr myChatMember;

    /**
     * @brief Optional. A chat member's status was updated in a chat.
     *
     * The bot must be an administrator in the chat and must explicitly specify
     * "chat_member" in the list of allowedUpdates to receive these updates.
     */
    ChatMemberUpdated::Ptr chatMember;

    /**
     * @brief Optional. A request to join the chat has been sent.
     *
     * The bot must have the canInviteUsers administrator right in the chat to
     * receive these updates.
     */
    ChatJoinRequest::Ptr chatJoinRequest;

    /**
     * @brief Optional. A chat boost was added or changed.
     *
     * The bot must be an administrator in the chat to receive these updates.
     */
    ChatBoostUpdated::Ptr chatBoost;

    /**
     * @brief Optional. A boost was removed from a chat.
     *
     * The bot must be an administrator in the chat to receive these updates.
     */
    ChatBoostRemoved::Ptr removedChatBoost;

    enum class Types {
        message = 1,
        edited_message = 1 << 1,
        channel_post = 1 << 2,
        edited_channel_post = 1 << 3,
        business_connection = 1 << 4,
        business_message = 1 << 5,
        edited_business_message = 1 << 6,
        deleted_business_messages = 1 << 7,
        message_reaction = 1 << 8,
        message_reaction_count = 1 << 9,
        inline_query = 1 << 10,
        chosen_inline_result = 1 << 11,
        callback_query = 1 << 12,
        shipping_query = 1 << 13,
        pre_checkout_query = 1 << 14,
        purchased_paid_media = 1 << 15,
        poll = 1 << 16,
        poll_answer = 1 << 17,
        my_chat_member = 1 << 18,
        chat_member = 1 << 19,
        chat_join_request = 1 << 20,
        chat_boost = 1 << 21,
        removed_chat_boost = 1 << 22,
    };
};
}  // namespace TgBot

inline constexpr TgBot::Update::Types operator|(
    const TgBot::Update::Types lhs, const TgBot::Update::Types rhs) noexcept {
    return static_cast<TgBot::Update::Types>(static_cast<int>(lhs) |
                                             static_cast<int>(rhs));
}

inline constexpr TgBot::Update::Types operator|=(
    TgBot::Update::Types& lhs, const TgBot::Update::Types rhs) noexcept {
    return lhs = lhs | rhs;
}

inline constexpr bool operator&(const TgBot::Update::Types& lhs,
                                const TgBot::Update::Types rhs) noexcept {
    return static_cast<bool>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

#endif  // TGBOT_UPDATE_H
