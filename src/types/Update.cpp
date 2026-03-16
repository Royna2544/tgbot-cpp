#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Update) {
    auto result = std::make_shared<Update>();
    parse(data, "update_id", &result->updateId);
    result->message = parse<Message>(data, "message");
    result->editedMessage = parse<Message>(data, "edited_message");
    result->channelPost = parse<Message>(data, "channel_post");
    result->editedChannelPost = parse<Message>(data, "edited_channel_post");
    result->businessConnection = parse<BusinessConnection>(data, "business_connection");
    result->businessMessage = parse<Message>(data, "business_message");
    result->editedBusinessMessage = parse<Message>(data, "edited_business_message");
    result->deletedBusinessMessages = parse<BusinessMessagesDeleted>(data, "deleted_business_messages");
    result->messageReaction = parse<MessageReactionUpdated>(data, "message_reaction");
    result->messageReactionCount = parse<MessageReactionCountUpdated>(data, "message_reaction_count");
    result->inlineQuery = parse<InlineQuery>(data, "inline_query");
    result->chosenInlineResult = parse<ChosenInlineResult>(data, "chosen_inline_result");
    result->callbackQuery = parse<CallbackQuery>(data, "callback_query");
    result->shippingQuery = parse<ShippingQuery>(data, "shipping_query");
    result->preCheckoutQuery = parse<PreCheckoutQuery>(data, "pre_checkout_query");
    result->purchasedPaidMedia = parse<PaidMediaPurchased>(data, "purchased_paid_media");
    result->poll = parse<Poll>(data, "poll");
    result->pollAnswer = parse<PollAnswer>(data, "poll_answer");
    result->myChatMember = parse<ChatMemberUpdated>(data, "my_chat_member");
    result->chatMember = parse<ChatMemberUpdated>(data, "chat_member");
    result->chatJoinRequest = parse<ChatJoinRequest>(data, "chat_join_request");
    result->chatBoost = parse<ChatBoostUpdated>(data, "chat_boost");
    result->removedChatBoost = parse<ChatBoostRemoved>(data, "removed_chat_boost");
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
        json.put("purchased_paid_media", object->purchasedPaidMedia);
        json.put("poll", object->poll);
        json.put("poll_answer", object->pollAnswer);
        json.put("my_chat_member", object->myChatMember);
        json.put("chat_member", object->chatMember);
        json.put("chat_join_request", object->chatJoinRequest);
        json.put("chat_boost", object->chatBoost);
        json.put("removed_chat_boost", object->removedChatBoost);
    }
    return json;
}

} // namespace TgBot
