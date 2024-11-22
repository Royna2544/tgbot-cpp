#ifndef TGBOT_HTTPPARSER_H
#define TGBOT_HTTPPARSER_H

#include <string>
#include <unordered_map>

#include "tgbot/net/HttpReqArg.h"
#include "tgbot/net/Url.h"

namespace TgBot {

namespace HttpParser {

std::string generateRequest(const Url& url, const HttpReqArg::Vec& args,
                            bool isKeepAlive = false);
std::string generateMultipartFormData(const HttpReqArg::Vec& args,
                                      const std::string& boundary);
std::string generateMultipartBoundary(const HttpReqArg::Vec& args);
std::string generateWwwFormUrlencoded(const HttpReqArg::Vec& args);
std::string generateResponse(const std::string& data,
                             const std::string& mimeType,
                             unsigned short statusCode,
                             const std::string& statusStr, bool isKeepAlive);
std::unordered_map<std::string, std::string> parseHeader(
    const std::string& data, bool isRequest);
std::string extractBody(const std::string& data);
};  // namespace HttpParser

}  // namespace TgBot

#endif  // TGBOT_HTTPPARSER_H
