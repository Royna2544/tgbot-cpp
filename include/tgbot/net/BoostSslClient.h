#ifndef TGBOT_BOOSTHTTPCLIENT_H
#define TGBOT_BOOSTHTTPCLIENT_H

#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "tgbot/net/HttpClient.h"
#include "tgbot/net/HttpParser.h"
#include "tgbot/net/HttpReqArg.h"
#include "tgbot/net/Url.h"

namespace TgBot {

/**
 * @brief This class makes http requests via boost::asio.
 *
 * @ingroup net
 */
class TGBOT_API BoostSslClient : public HttpClient {
   public:
    explicit BoostSslClient(std::chrono::seconds timeout = kDefaultTimeout);
    ~BoostSslClient() override;

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
#if BOOST_VERSION >= 108700
    mutable boost::asio::io_context _ioService;
#else
    mutable boost::asio::io_service _ioService;
#endif
    std::thread _ioServiceThread;
};

}  // namespace TgBot

#endif  // TGBOT_BOOSTHTTPCLIENT_H
