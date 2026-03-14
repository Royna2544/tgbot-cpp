#include "tgbot/EventHandler.h"
#include "tgbot/tools/StringTools.h"

namespace TgBot {

void EventHandler::handleUpdate(const Update::Ptr& update) const {
    if (update->message) {
        handleMessage(*update->message);
    }
    if (update->editedMessage) {
        _broadcaster->broadcastEditedMessage(*update->editedMessage);
    }
    if (update->channelPost) {
        handleMessage(*update->channelPost);
    }
    if (update->editedChannelPost) {
        _broadcaster->broadcastEditedMessage(*update->editedChannelPost);
    }
    if (update->businessConnection) {
        _broadcaster->broadcastBusinessConnection(*update->businessConnection);
    }
    if (update->businessMessage) {
        _broadcaster->broadcastBusinessMessage(*update->businessMessage);
    }
    if (update->editedBusinessMessage) {
        _broadcaster->broadcastEditedBusinessMessage(*update->editedBusinessMessage);
    }
    if (update->deletedBusinessMessages) {
        _broadcaster->broadcastDeletedBusinessMessages(*update->deletedBusinessMessages);
    }
    if (update->messageReaction) {
        _broadcaster->broadcastMessageReactionUpdated(*update->messageReaction);
    }
    if (update->messageReactionCount) {
        _broadcaster->broadcastMessageReactionCountUpdated(*update->messageReactionCount);
    }
    if (update->inlineQuery) {
        _broadcaster->broadcastInlineQuery(*update->inlineQuery);
    }
    if (update->chosenInlineResult) {
        _broadcaster->broadcastChosenInlineResult(*update->chosenInlineResult);
    }
    if (update->callbackQuery) {
        _broadcaster->broadcastCallbackQuery(*update->callbackQuery);
    }
    if (update->shippingQuery) {
        _broadcaster->broadcastShippingQuery(*update->shippingQuery);
    }
    if (update->preCheckoutQuery) {
        _broadcaster->broadcastPreCheckoutQuery(*update->preCheckoutQuery);
    }
    if (update->poll) {
        _broadcaster->broadcastPoll(*update->poll);
    }
    if (update->pollAnswer) {
        _broadcaster->broadcastPollAnswer(*update->pollAnswer);
    }
    if (update->myChatMember) {
        _broadcaster->broadcastMyChatMember(*update->myChatMember);
    }
    if (update->chatMember) {
        _broadcaster->broadcastChatMember(*update->chatMember);
    }
    if (update->chatJoinRequest) {
        _broadcaster->broadcastChatJoinRequest(*update->chatJoinRequest);
    }
    if (update->chatBoost) {
        _broadcaster->broadcastChatBoostUpdated(*update->chatBoost);
    }
    if (update->removedChatBoost) {
        _broadcaster->broadcastRemovedChatBoost(*update->removedChatBoost);
    }
}

void EventHandler::handleMessage(const Message::Ptr& message) const {
    _broadcaster->broadcastAnyMessage(message);

    if (message->text && StringTools::startsWith(message->text.value(), "/")) {
        std::size_t splitPosition;
        std::size_t spacePosition = message->text->find(' ');
        std::size_t atSymbolPosition = message->text->find('@');
        if (spacePosition == std::string::npos) {
            if (atSymbolPosition == std::string::npos) {
                splitPosition = message->text->size();
            } else {
                splitPosition = atSymbolPosition;
            }
        } else if (atSymbolPosition == std::string::npos) {
            splitPosition = spacePosition;
        } else {
            splitPosition = std::min(spacePosition, atSymbolPosition);
        }
        std::string command = message->text->substr(1, splitPosition - 1);
        if (!_broadcaster->broadcastCommand(command, message)) {
            _broadcaster->broadcastUnknownCommand(message);
        }
    } else {
        _broadcaster->broadcastNonCommandMessage(message);
    }
}

}
