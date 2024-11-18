#ifndef TGBOT_EVENTHANDLER_H
#define TGBOT_EVENTHANDLER_H

#include "tgbot/EventBroadcaster.h"
#include "tgbot/types/Update.h"

namespace TgBot {

class TGBOT_API EventHandler {
   public:
    explicit EventHandler(EventBroadcaster* broadcaster)
        : _broadcaster(broadcaster) {}

    void handleUpdate(const Update::Ptr& update) const;

   private:
    EventBroadcaster* _broadcaster;

    void handleMessage(const Message::Ptr& message) const;
};

}  // namespace TgBot

#endif  // TGBOT_EVENTHANDLER_H
