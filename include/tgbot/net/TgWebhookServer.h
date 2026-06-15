#ifndef TGBOT_TGWEBHOOKSERVER_H
#define TGBOT_TGWEBHOOKSERVER_H

#include <memory>
#include <string>

#include "tgbot/export.h"

namespace TgBot {

class EventHandler;

/**
 * @brief Base HTTP server for receiving Telegram Update objects via webhooks.
 *
 * Implemented on top of cpp-httplib. Use the TgWebhookTcpServer (TCP) or
 * TgWebhookLocalServer (UNIX socket) subclasses, or Bot::createWebHookTcp /
 * Bot::createWebHookLocal.
 *
 * @ingroup net
 */
class TGBOT_API TgWebhookServer {
   public:
    virtual ~TgWebhookServer();

    TgWebhookServer(const TgWebhookServer&) = delete;
    TgWebhookServer& operator=(const TgWebhookServer&) = delete;

    /**
     * @brief Starts listening for connections. Blocks until stop() is called.
     */
    void start();

    /**
     * @brief Stops the server.
     */
    void stop();

   protected:
    struct Bind {
        bool unixSocket = false;
        std::string host;
        unsigned short port = 0;
        std::string socketPath;
    };

    TgWebhookServer(Bind bind, std::string path, EventHandler* eventHandler);

   private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};

}  // namespace TgBot

#endif  // TGBOT_TGWEBHOOKSERVER_H
