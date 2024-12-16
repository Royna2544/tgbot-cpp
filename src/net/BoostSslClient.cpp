#include "tgbot/net/BoostSslClient.h"

#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/verify_mode.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>
#include <chrono>
#include <initializer_list>
#include <string>

#include "tgbot/TgException.h"
#include "tgbot/net/HttpClient.h"
#include "tgbot/net/HttpReqArg.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace TgBot {

BoostSslClient::BoostSslClient(std::chrono::seconds timeout)
    : HttpClient(timeout) {
    // Start the io_service
    _ioServiceThread = std::thread([this]() {
        auto work = boost::asio::make_work_guard(_ioService);
        _ioService.run();
    });
}

BoostSslClient::~BoostSslClient() {
    _ioService.stop();
    _ioServiceThread.join();
}

std::string BoostSslClient::makeRequest(const Url& url,
                                        const HttpReqArg::Vec& args) const {
    constexpr static int kBufferSize = 1 << 12;

    tcp::resolver resolver(_ioService);
    ssl::context context(ssl::context::tlsv13_client);
    context.set_default_verify_paths();

    if (auto cert = getServerCert(); cert) {
        context.load_verify_file(cert->string());
    }

    // Calculate the end time for the timeout
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now() + timeout();

    assert(_ioService.stopped() == false);

    // Keep the io_service running
    auto work = boost::asio::make_work_guard(_ioService);

    // Create the socket
    ssl::stream<tcp::socket> socket(_ioService, context);
#if BOOST_VERSION >= 108700
    auto ip = resolver.async_resolve(url.host, "443", boost::asio::use_future);
#else
    tcp::resolver::query query(url.host, "443");
    auto ip = resolver.async_resolve(query, boost::asio::use_future);
#endif
    if (ip.wait_until(end) != std::future_status::ready) {
        throw NetworkException(NetworkException::State::Connect,
                               "TIMEOUT on resolve " + url.host);
    }

    boost::asio::ip::tcp::resolver::results_type results;
    try {
        results = ip.get();
    } catch (const boost::system::system_error& e) {
        throw NetworkException(NetworkException::State::Connect, e.what());
    }

    std::string result;
    auto tcp =
        async_connect(socket.lowest_layer(), results, boost::asio::use_future);

    if (tcp.wait_until(end) != std::future_status::ready) {
        throw NetworkException(NetworkException::State::Connect,
                               "TIMEOUT on connect to " + url.host);
    } else {
        try {
            tcp.get();
        } catch (const boost::system::system_error& e) {
            throw NetworkException(NetworkException::State::Connect, e.what());
        }
    }

    socket.set_verify_mode(ssl::verify_peer);
#if BOOST_VERSION >= 108700
    socket.set_verify_callback(ssl::host_name_verification(url.host));
#else
    socket.set_verify_callback(ssl::rfc2818_verification(url.host));
#endif

    auto handshake = socket.async_handshake(ssl::stream<tcp::socket>::client,
                                            boost::asio::use_future);
    if (handshake.wait_until(end) != std::future_status::ready) {
        throw NetworkException(NetworkException::State::Connect,
                               "TIMEOUT on handshake with " + url.host);
    } else {
        try {
            handshake.get();
        } catch (const boost::system::system_error& e) {
            throw NetworkException(NetworkException::State::Handshake,
                                   e.what());
        }
    }

    std::string requestText = HttpParser::generateRequest(url, args, false);
    auto write =
        async_write(socket, buffer(requestText), boost::asio::use_future);
    if (write.wait_until(end) != std::future_status::ready) {
        throw NetworkException(NetworkException::State::Write,
                               "TIMEOUT on write to " + url.host);
    } else if (write.get() != requestText.size()) {
        throw NetworkException(NetworkException::State::Write,
                               "Failed to write all data to " + url.host);
    }

    std::stringstream response;
    std::array<char, kBufferSize> resultBuffer{};
    boost::system::error_code error;

    while (!error) {
        auto readResult = socket.async_read_some(buffer(resultBuffer),
                                                 boost::asio::use_future);
        if (readResult.wait_until(end) != std::future_status::ready) {
#if BOOST_VERSION >= 108700
            _ioService.restart();
#else
            _ioService.reset();
#endif
            throw NetworkException(NetworkException::State::Read,
                                   "TIMEOUT on read from " + url.host);
        }
        try {
            size_t readBytes = readResult.get();
            if (readBytes == 0) {
                break;
            }
            response << std::string_view(resultBuffer.data(), readBytes);
        } catch (const boost::system::system_error& e) {
            static const std::vector<boost::system::error_code> kIgnoredErrors =
                {
                    boost::asio::error::eof,
                    boost::asio::error::connection_reset,
                    boost::asio::ssl::error::stream_truncated,
                };
            if (kIgnoredErrors.end() != std::find(kIgnoredErrors.begin(),
                                                  kIgnoredErrors.end(),
                                                  e.code())) {
                break;
            }
            throw NetworkException(NetworkException::State::Read, e.what());
        }
    }

    return HttpParser::extractBody(response.str());
}

}  // namespace TgBot
