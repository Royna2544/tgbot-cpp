#ifndef TGBOT_TGWEBHOOKLOCALSERVER_H
#define TGBOT_TGWEBHOOKLOCALSERVER_H

#ifndef _WIN32

#include <string>
#include <utility>

#include "tgbot/net/TgWebhookServer.h"

namespace TgBot {

/**
 * @brief Sets up an HTTP server for receiving Telegram Update objects over a
 * UNIX domain socket.
 *
 * @ingroup net
 */
class TGBOT_API TgWebhookLocalServer : public TgWebhookServer {
   public:
    TgWebhookLocalServer(std::string unixSocketPath, std::string path,
                         EventHandler* eventHandler)
        : TgWebhookServer(
              Bind{true, std::string(), 0, std::move(unixSocketPath)},
              std::move(path), eventHandler) {}
};

}  // namespace TgBot

#endif  // _WIN32

#endif  // TGBOT_TGWEBHOOKLOCALSERVER_H
