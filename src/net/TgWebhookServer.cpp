#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#ifndef CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#endif
#include <httplib.h>

#include <exception>
#include <nlohmann/json.hpp>
#include <utility>

#include "tgbot/EventHandler.h"
#include "tgbot/TgTypeParser.h"
#include "tgbot/net/TgWebhookServer.h"
#include "tgbot/types/Update.h"

namespace TgBot {

struct TgWebhookServer::Impl {
    Bind bind;
    std::string path;
    EventHandler* eventHandler;
    httplib::Server server;

    Impl(Bind bind_, std::string path_, EventHandler* eventHandler_)
        : bind(std::move(bind_)),
          path(std::move(path_)),
          eventHandler(eventHandler_) {
        server.Post(path, [this](const httplib::Request& req,
                                 httplib::Response& res) {
            try {
                nlohmann::json update = nlohmann::json::parse(req.body);
                eventHandler->handleUpdate(parse<Update>(update));
            } catch (const std::exception&) {
                // Ignore malformed payloads; always answer 200 so Telegram
                // does not keep retrying the delivery.
            }
            res.set_content("", "text/plain");
        });
    }

    void start() {
        if (bind.unixSocket) {
            server.set_address_family(AF_UNIX);
            server.listen(bind.socketPath, 80);
        } else {
            server.listen(bind.host, bind.port);
        }
    }

    void stop() { server.stop(); }
};

TgWebhookServer::TgWebhookServer(Bind bind, std::string path,
                                 EventHandler* eventHandler)
    : _impl(std::make_unique<Impl>(std::move(bind), std::move(path),
                                   eventHandler)) {}

TgWebhookServer::~TgWebhookServer() = default;

void TgWebhookServer::start() { _impl->start(); }

void TgWebhookServer::stop() { _impl->stop(); }

}  // namespace TgBot
