#ifndef TGBOT_HTTPLIBCLIENT_H
#define TGBOT_HTTPLIBCLIENT_H

#include <chrono>
#include <memory>
#include <mutex>
#include <string>

#include "tgbot/net/HttpClient.h"
#include "tgbot/net/HttpReqArg.h"
#include "tgbot/net/Url.h"

namespace httplib {
class Client;
}

namespace TgBot {

/**
 * @brief This class makes http requests via cpp-httplib.
 *
 * It is the default HttpClient used by Bot and supports HTTPS out of the box
 * using the system's trusted CA store (or a custom certificate set through
 * HttpClient::setServerCert). The underlying connection is kept alive and
 * reused across requests; access is serialized so a single instance can be
 * shared between threads.
 *
 * @ingroup net
 */
class TGBOT_API HttplibClient : public HttpClient {
   public:
    explicit HttplibClient(std::chrono::seconds timeout = kDefaultTimeout);
    ~HttplibClient() override;

    /**
     * @brief Sends a request to the url.
     *
     * If there's no args specified, a GET request will be sent, otherwise a
     * POST request will be sent. If at least 1 arg is marked as file, the
     * content type of a request will be multipart/form-data, otherwise it will
     * be application/x-www-form-urlencoded.
     */
    std::string makeRequest(const Url& url,
                            const HttpReqArg::Vec& args) const override;

   private:
    mutable std::mutex _mutex;
    mutable std::unique_ptr<httplib::Client> _client;
    mutable std::string _clientBase;
};

}  // namespace TgBot

#endif  // TGBOT_HTTPLIBCLIENT_H
