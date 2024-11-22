#ifndef TGBOT_TGWEBHOOKTCPSERVER_H
#define TGBOT_TGWEBHOOKTCPSERVER_H

#include <tgbot/net/TgWebhookServer.h>

#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace TgBot {

/**
 * This class setups HTTP server for receiving Telegram Update objects from tcp
 * connections.
 * @ingroup net
 */
class TgWebhookTcpServer : public TgWebhookServer<boost::asio::ip::tcp> {
   public:
    TgWebhookTcpServer(unsigned short port, const std::string& path,
                       EventHandler* eventHandler)
        : TgWebhookServer<boost::asio::ip::tcp>(
              boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port),
              path, eventHandler) {}
};

}  // namespace TgBot

#endif  // TGBOT_TGWEBHOOKTCPSERVER_H
