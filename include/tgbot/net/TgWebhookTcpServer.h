#ifndef TGBOT_TGWEBHOOKTCPSERVER_H
#define TGBOT_TGWEBHOOKTCPSERVER_H

#include <string>
#include <utility>

#include "tgbot/net/TgWebhookServer.h"

namespace TgBot {

/**
 * @brief Sets up an HTTP server for receiving Telegram Update objects over TCP.
 *
 * @ingroup net
 */
class TGBOT_API TgWebhookTcpServer : public TgWebhookServer {
   public:
    TgWebhookTcpServer(unsigned short port, std::string path,
                       EventHandler* eventHandler,
                       std::string host = "0.0.0.0")
        : TgWebhookServer(Bind{false, std::move(host), port, std::string()},
                          std::move(path), eventHandler) {}
};

}  // namespace TgBot

#endif  // TGBOT_TGWEBHOOKTCPSERVER_H
