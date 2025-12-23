#ifndef TGBOT_TGHTTPSERVER_H
#define TGBOT_TGHTTPSERVER_H

#include <nlohmann/json.hpp>

#include <string>
#include <unordered_map>
#include <utility>

#include "tgbot/EventHandler.h"
#include "tgbot/TgTypeParser.h"
#include "tgbot/net/HttpServer.h"

namespace TgBot {

template <typename Protocol>
class TgWebhookServer : public HttpServer<Protocol> {
   public:
    TgWebhookServer(
        const typename boost::asio::basic_socket_acceptor<
            Protocol>::endpoint_type &endpoint,
        const typename HttpServer<Protocol>::ServerHandler &handler) = delete;

    TgWebhookServer(const typename boost::asio::basic_socket_acceptor<
                        Protocol>::endpoint_type &endpoint,
                    std::string path, EventHandler *eventHandler)
        : HttpServer<Protocol>(
              endpoint,
              [this](const std::string &_1,
                     const std::unordered_map<std::string, std::string> &_2) {
                  return _handle(_1, _2);
              }),
          _path(std::move(path)),
          _eventHandler(eventHandler) {}

   private:
    std::string _handle(
        const std::string &data,
        const std::unordered_map<std::string, std::string> &headers) {
        if (headers.at("_method") == "POST" && headers.at("_path") == _path) {
            nlohmann::json update = nlohmann::json::parse(data);
            _eventHandler->handleUpdate(parse<Update>(update));
        }
        return HttpParser::generateResponse("", "text/plain", 200, "OK", false);
    }

    std::string _path;
    EventHandler *_eventHandler;
};

}  // namespace TgBot

#endif  // TGBOT_TGHTTPSERVER_H
