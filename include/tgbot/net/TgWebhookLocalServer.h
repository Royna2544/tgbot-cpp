#ifndef TGBOT_TGWEBHOOKLOCALSERVER_H
#define TGBOT_TGWEBHOOKLOCALSERVER_H

#include <boost/asio/local/basic_endpoint.hpp>

#ifdef BOOST_ASIO_HAS_LOCAL_SOCKETS

#include <tgbot/net/TgWebhookServer.h>

#include <string>

namespace TgBot {

/**
 * @brief This class setups HTTP server for receiving Telegram Update objects
 * from unix socket.
 *
 * @ingroup net
 */
class TgWebhookLocalServer
    : public TgWebhookServer<boost::asio::local::stream_protocol> {
   public:
    TgWebhookLocalServer(const std::string& unixSocketPath,
                         const std::string& path, EventHandler* eventHandler)
        : TgWebhookServer<boost::asio::local::stream_protocol>(
              boost::asio::local::stream_protocol::endpoint(unixSocketPath),
              path, eventHandler) {}
};

}  // namespace TgBot

#endif  // BOOST_ASIO_HAS_LOCAL_SOCKETS

#endif  // TGBOT_TGWEBHOOKLOCALSERVER_H
