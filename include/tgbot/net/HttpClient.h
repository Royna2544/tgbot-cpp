#ifndef TGBOT_HTTPCLIENT_H
#define TGBOT_HTTPCLIENT_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include "tgbot/net/HttpReqArg.h"
#include "tgbot/net/Url.h"

namespace TgBot {

/**
 * @brief This class makes http requests.
 *
 * @ingroup net
 */
class TGBOT_API HttpClient {
    std::chrono::seconds _timeout = kDefaultTimeout;
    std::optional<std::filesystem::path> _caCertPath;

   public:
    virtual ~HttpClient() = default;
    explicit HttpClient(const std::chrono::seconds timeout)
        : _timeout(timeout){};

    /**
     * @brief Maximum number of makeRequest() retries before giving up and
     * throwing an exception.
     */
    constexpr static int kRequestMaxRetries = 3;
    /**
     * @brief Backoff duration between retries, in seconds.
     */
    constexpr static std::chrono::seconds kRequestBackoff{1};
    /**
     * @brief Default timeout, in seconds
     */
    constexpr static std::chrono::seconds kDefaultTimeout{10};

    /**
     * @brief Sends a request to the url.
     *
     * If there's no args specified, a GET request will be sent, otherwise a
     * POST request will be sent. If at least 1 arg is marked as file, the
     * content type of a request will be multipart/form-data, otherwise it will
     * be application/x-www-form-urlencoded.
     */
    virtual std::string makeRequest(const Url& url,
                                    const HttpReqArg::Vec& args) const = 0;

    /**
     * @brief Set the certificate required for the server to be authenticated
     * with HTTPS
     *
     * Specify the file path that contains the server's certs, if using server
     * without a globally trusted CA's cert.
     */
    void setServerCert(std::filesystem::path caCertPath) {
        _caCertPath = std::move(caCertPath);
    }

    /**
     * @brief Get the certificate required for the server to be authenticated
     * with HTTPS
     *
     * Empty string, if not set.
     */
    [[nodiscard]] std::optional<std::filesystem::path> getServerCert() const {
        return _caCertPath;
    }

    [[nodiscard]] std::chrono::seconds timeout() const { return _timeout; }
    void timeout(std::chrono::seconds newTimeout) { _timeout = newTimeout; }
};

}  // namespace TgBot

#endif  // TGBOT_HTTPCLIENT_H
