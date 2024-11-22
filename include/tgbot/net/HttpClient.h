#ifndef TGBOT_HTTPCLIENT_H
#define TGBOT_HTTPCLIENT_H

#include "tgbot/net/Url.h"
#include "tgbot/net/HttpReqArg.h"

#include <string>
#include <vector>
#include <cstdint>

namespace TgBot {

/**
 * @brief This class makes http requests.
 *
 * @ingroup net
 */
class TGBOT_API HttpClient {

public:
    virtual ~HttpClient() = default;

    constexpr static std::int32_t kDefaultTimeout = 10;

    /**
     * @brief Sends a request to the url.
     *
     * If there's no args specified, a GET request will be sent, otherwise a POST request will be sent.
     * If at least 1 arg is marked as file, the content type of a request will be multipart/form-data, otherwise it will be application/x-www-form-urlencoded.
     */
    virtual std::string makeRequest(const Url& url, const HttpReqArg::Vec& args) const = 0;

    std::int32_t _timeout = 25;

    std::int32_t timeout() const {
      return _timeout;
    }
    void timeout(std::int32_t newTimeout) {
      _timeout = newTimeout + 5;
    }

    /**
      * @brief Get the maximum number of makeRequest() retries before giving up and throwing an exception.
      */
    virtual int getRequestMaxRetries() const {
        return requestMaxRetries;
    }

    /**
      * @brief Get the makeRequest() backoff duration between retries, in seconds.
      */
    virtual int getRequestBackoff() const {
        return requestBackoff;
    }

private:
    int requestMaxRetries = 3;
    int requestBackoff = 1;
};

}


#endif //TGBOT_HTTPCLIENT_H
