#ifndef TGBOT_HTTPSERVER_H
#define TGBOT_HTTPSERVER_H

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstddef>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "tgbot/net/HttpParser.h"

namespace TgBot {

/**
 * @brief This class handles HTTP requests from the Internet.
 *
 * @ingroup net
 */
template <typename Protocol>
class HttpServer {
   protected:
    class Connection;

   public:
    using ServerHandler = std::function<std::string(
        const std::string&,
        const std::unordered_map<std::string, std::string>&)>;

    HttpServer(const typename boost::asio::basic_socket_acceptor<
                   Protocol>::endpoint_type& endpoint,
               ServerHandler handler)
        : _acceptor(_ioService, endpoint),
          _socket(_ioService),
          _handler(std::move(handler)) {}

    /**
     * @brief Starts receiving new connections.
     */
    void start() {
        _startAccept();
        _ioService.run();
    }

    /**
     * @brief Stops receiving new connections.
     */
    void stop() { _ioService.stop(); }

   protected:
    class Connection : public std::enable_shared_from_this<Connection> {
       public:
        Connection(boost::asio::basic_stream_socket<Protocol> socket,
                   ServerHandler handler)
            : _socket(std::move(socket)), _handler(std::move(handler)) {}

        void start() { _readHeader(); }

       protected:
        boost::asio::basic_stream_socket<Protocol> _socket;
        ServerHandler _handler;

        void _readHeader() {
            auto self(this->shared_from_this());

            auto data(std::make_shared<boost::asio::streambuf>());
            data->prepare(1024);

            boost::asio::async_read_until(
                _socket, *data, "\r\n\r\n",
                [self, data](const boost::system::error_code& e,
                             std::size_t n) {
                    if (e) {
                        std::cout
                            << "error in HttpServer::Connection#_readHeader: "
                            << e << std::endl;
                        return;
                    }

                    boost::asio::streambuf::const_buffers_type bufs =
                        data->data();
                    std::string dataAsString(
                        boost::asio::buffers_begin(bufs),
                        boost::asio::buffers_begin(bufs) + n);

                    auto headers(std::make_shared<
                                 std::unordered_map<std::string, std::string>>(
                        HttpParser::parseHeader(dataAsString, true)));

                    unsigned long long size;
                    auto contentLengthIter = headers->find("Content-Length");
                    if (contentLengthIter != headers->end()) {
                        size = std::stoull(contentLengthIter->second);
                    } else {
                        size = 0;
                    }

                    if (size == 0) {
                        std::string answer = HttpParser::generateResponse(
                            "Bad request", "text/plain", 400, "Bad request",
                            false);
                        boost::asio::async_write(
                            self->_socket, boost::asio::buffer(answer),
                            [](const boost::system::error_code&, std::size_t) {
                            });
                        return;
                    }

                    data->consume(n);
                    self->_readBody(data, size, headers);
                });
        }

        void _readBody(
            std::shared_ptr<boost::asio::streambuf> data,
            unsigned long long size,
            std::shared_ptr<std::unordered_map<std::string, std::string>>
                headers) {
            auto self(this->shared_from_this());

            data->prepare(size);

            boost::asio::async_read(
                _socket, *data,
                boost::asio::transfer_exactly(size - data->size()),
                [self, data, size, headers](const boost::system::error_code& e,
                                            std::size_t) {
                    if (e) {
                        std::cout
                            << "error in HttpServer::Connection#_readBody: "
                            << e << std::endl;
                        return;
                    }

                    boost::asio::streambuf::const_buffers_type bufs =
                        data->data();
                    std::string dataAsString(
                        boost::asio::buffers_begin(bufs),
                        boost::asio::buffers_begin(bufs) + size);

                    std::string answer;
                    try {
                        answer = self->_handler(dataAsString, *headers);
                    } catch (std::exception& e) {
                        std::cout << "error in "
                                     "HttpServer::Connection#_readBody answer: "
                                  << e.what() << std::endl;
                        answer = HttpParser::generateResponse(
                            "Internal server error", "text/plain", 500,
                            "Internal server error", false);
                    }
                    boost::asio::async_write(
                        self->_socket, boost::asio::buffer(answer),
                        [](const boost::system::error_code&, std::size_t) {});

                    self->_socket.close();
                });
        }
    };

    void _startAccept() {
        _acceptor.async_accept(
            _socket, [this](const boost::system::error_code& e) {
                if (e) {
                    std::cout << "error in HttpServer: " << e << std::endl;
                    _startAccept();
                    return;
                }

                auto connection(
                    std::make_shared<Connection>(std::move(_socket), _handler));
                connection->start();

                _startAccept();
            });
    }

#if BOOST_VERSION >= 108700
    boost::asio::io_context _ioService;
#else
    boost::asio::io_service _ioService;
#endif
    boost::asio::basic_socket_acceptor<Protocol> _acceptor;
    boost::asio::basic_stream_socket<Protocol> _socket;
    ServerHandler _handler;
};

}  // namespace TgBot

#endif  // TGBOT_HTTPSERVER_H
