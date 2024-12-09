#include "tgbot/net/BoostHttpOnlySslClient.h"

#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/verify_mode.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>
#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "tgbot/TgException.h"
#include "tgbot/net/HttpClient.h"
#include "tgbot/net/HttpReqArg.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace TgBot {

BoostHttpOnlySslClient::BoostHttpOnlySslClient(std::chrono::seconds timeout)
    : HttpClient(timeout) {}

BoostHttpOnlySslClient::~BoostHttpOnlySslClient() = default;

std::string BoostHttpOnlySslClient::_makeRequest(
    const Url& url, const HttpReqArg::Vec& args) const {
    tcp::resolver resolver(_ioService);
    tcp::resolver::query query(url.host, "443");
#ifdef TGBOT_LP64
    constexpr static int kIncreasedBufferSize = 1 << 16;
#else
    constexpr static int kIncreasedBufferSize = 1 << 15;
#endif

    ssl::context context(ssl::context::tlsv13_client);
    context.set_default_verify_paths();

    if (auto cert = getServerCert(); cert) {
        context.load_verify_file(cert->string());
    }

    ssl::stream<tcp::socket> socket(_ioService, context);

    connect(socket.lowest_layer(), resolver.resolve(query));

#ifdef TGBOT_DISABLE_NAGLES_ALGORITHM
    socket.lowest_layer().set_option(tcp::no_delay(true));
#endif  // TGBOT_DISABLE_NAGLES_ALGORITHM
#ifdef TGBOT_CHANGE_SOCKET_BUFFER_SIZE
    socket.lowest_layer().set_option(
        socket_base::send_buffer_size(kIncreasedBufferSize));
    socket.lowest_layer().set_option(
        socket_base::receive_buffer_size(kIncreasedBufferSize));
#endif  // TGBOT_CHANGE_SOCKET_BUFFER_SIZE
    socket.set_verify_mode(ssl::verify_peer);
    socket.set_verify_callback(ssl::rfc2818_verification(url.host));

    socket.handshake(ssl::stream<tcp::socket>::client);

    std::string requestText = HttpParser::generateRequest(url, args, false);
    write(socket, buffer(requestText.c_str(), requestText.length()));

    fd_set fileDescriptorSet;
    struct timeval timeStruct {};

    // set the timeout to 20 seconds
    timeStruct.tv_sec = timeout().count();
    FD_ZERO(&fileDescriptorSet);

    // We'll need to get the underlying native socket for this select call, in
    // order to add a simple timeout on the read:

    int nativeSocket = static_cast<int>(socket.lowest_layer().native_handle());

    FD_SET(nativeSocket, &fileDescriptorSet);
    select(nativeSocket + 1, &fileDescriptorSet, nullptr, nullptr, &timeStruct);

    if (!FD_ISSET(nativeSocket, &fileDescriptorSet)) {  // timeout

        std::string sMsg("TIMEOUT on read client data. Client IP: ");

        sMsg.append(
            socket.next_layer().remote_endpoint().address().to_string());
        _ioService.reset();

        throw NetworkException(sMsg);
    }

    std::stringstream response;

#ifdef TGBOT_CHANGE_READ_BUFFER_SIZE
    std::array<char, kIncreasedBufferSize> buff{};
#else
    std::array<char, 1 << 10> buff{};
#endif  // TGBOT_CHANGE_READ_BUFFER_SIZE

    boost::system::error_code error;
    while (!error) {
        std::size_t bytes = read(socket, buffer(buff), error);
        response << std::string_view(buff.data(), bytes);
    }

    return HttpParser::extractBody(response.str());
}

std::string BoostHttpOnlySslClient::makeRequest(
    const Url& url, const HttpReqArg::Vec& args) const {
    try {
        return _makeRequest(url, args);
    } catch (const boost::wrapexcept<boost::system::system_error>& ex) {
        throw NetworkException(ex.what());
    }
}

}  // namespace TgBot
