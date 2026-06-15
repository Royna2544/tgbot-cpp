#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#ifndef CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#endif
#include <httplib.h>

#include <string>

#include "tgbot/TgException.h"
#include "tgbot/net/HttplibClient.h"

namespace TgBot {

HttplibClient::HttplibClient(std::chrono::seconds timeout)
    : HttpClient(timeout) {}

HttplibClient::~HttplibClient() = default;

namespace {

[[noreturn]] void throwNetworkError(const httplib::Result& res,
                                    const std::string& host) {
    if (res) {
        throw NetworkException(
            NetworkException::State::Read,
            "HTTP status " + std::to_string(res->status) + " from " + host);
    }
    throw NetworkException(
        NetworkException::State::Connect,
        "cpp-httplib error: " + httplib::to_string(res.error()) + " (" + host +
            ")");
}

}  // namespace

std::string HttplibClient::makeRequest(const Url& url,
                                       const HttpReqArg::Vec& args) const {
    httplib::Client client(url.protocol + "://" + url.host);

    const auto timeoutSecs = timeout().count();
    client.set_connection_timeout(timeoutSecs);
    client.set_read_timeout(timeoutSecs);
    client.set_write_timeout(timeoutSecs);
    client.set_follow_location(true);
    client.set_keep_alive(true);

    // HTTPS certificate verification: httplib falls back to the system's
    // default verify paths when no explicit CA certificate is configured.
    if (auto cert = getServerCert(); cert) {
        client.set_ca_cert_path(cert->string().c_str());
    }
    client.enable_server_certificate_verification(true);

    httplib::Result res;
    if (args.empty()) {
        std::string path = url.path;
        if (!url.query.empty()) {
            path += "?" + url.query;
        }
        res = client.Get(path);
    } else {
        bool hasFile = false;
        for (const auto& arg : args) {
            if (arg->isFile()) {
                hasFile = true;
                break;
            }
        }

        if (hasFile) {
            httplib::UploadFormDataItems items;
            items.reserve(args.size());
            for (const auto& arg : args) {
                httplib::UploadFormData item;
                item.name = arg->name;
                item.content = arg->value;
                if (arg->isFile()) {
                    const auto* file =
                        static_cast<const HttpReqArgFile*>(arg.get());
                    item.filename = file->fileName;
                    item.content_type = file->mimeType;
                }
                items.push_back(std::move(item));
            }
            res = client.Post(url.path, items);
        } else {
            httplib::Params params;
            for (const auto& arg : args) {
                params.emplace(arg->name, arg->value);
            }
            res = client.Post(url.path, params);
        }
    }

    if (!res || res->status < 200 || res->status >= 300) {
        // Telegram returns a JSON error body together with a 4xx status for
        // API-level failures; surface that body so the Api layer can parse it.
        if (res && !res->body.empty()) {
            return res->body;
        }
        throwNetworkError(res, url.host);
    }

    return res->body;
}

}  // namespace TgBot
