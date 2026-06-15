#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#ifndef CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#endif
#include <httplib.h>

#include <exception>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "tgbot/EventHandler.h"
#include "tgbot/Logger.h"
#include "tgbot/TgTypeParser.h"
#include "tgbot/net/TgWebhookServer.h"
#include "tgbot/types/Update.h"

namespace TgBot {

namespace {

// httplib treats route patterns as regular expressions; escape the path so it
// is matched literally (bot tokens and custom paths may contain regex
// metacharacters).
std::string escapeRegex(const std::string& path) {
    static const std::string specials = R"(.^$|()[]{}*+?\)";
    std::string escaped;
    escaped.reserve(path.size());
    for (char c : path) {
        if (specials.find(c) != std::string::npos) {
            escaped += '\\';
        }
        escaped += c;
    }
    return escaped;
}

}  // namespace

struct TgWebhookServer::Impl {
    Bind bind;
    std::string path;
    EventHandler* eventHandler;
    httplib::Server server;

    Impl(Bind bind_, std::string path_, EventHandler* eventHandler_)
        : bind(std::move(bind_)),
          path(std::move(path_)),
          eventHandler(eventHandler_) {
        server.Post(escapeRegex(path), [this](const httplib::Request& req,
                                               httplib::Response& res) {
            try {
                nlohmann::json update = nlohmann::json::parse(req.body);
                eventHandler->handleUpdate(parse<Update>(update));
            } catch (const std::exception& e) {
                // Log but always answer 200 so Telegram does not keep retrying
                // the delivery of a payload the handler cannot process.
                detail::log(LogLevel::Error,
                            std::string("Webhook handler error: ") + e.what());
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
