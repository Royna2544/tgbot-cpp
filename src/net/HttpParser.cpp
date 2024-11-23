#include "tgbot/net/HttpParser.h"

#include <boost/algorithm/string.hpp>
#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "tgbot/tools/StringTools.h"

namespace TgBot {

std::string HttpParser::generateRequest(const Url& url,
                                        const HttpReqArg::Vec& args,
                                        bool isKeepAlive) {
    std::stringstream result;
    if (args.empty()) {
        result << "GET ";
    } else {
        result << "POST ";
    }
    result << url.path;
    if (!url.query.empty()) {
        result << "?" << url.query;
    }
    result << " HTTP/1.1" << detail::CRLF;
    result << "Host: " << url.host << detail::CRLF;
    result << "Connection: " << (isKeepAlive ? "keep-alive" : "close")
           << detail::CRLF;
    if (args.empty()) {
        result << detail::CRLF;
    } else {
        std::string requestData;
        std::string bondary = generateMultipartBoundary(args);
        if (bondary.empty()) {
            result << "Content-Type: application/x-www-form-urlencoded"
                   << detail::CRLF;
            requestData = generateWwwFormUrlencoded(args);
        } else {
            result << "Content-Type: multipart/form-data; boundary=" << bondary
                   << detail::CRLF;
            requestData = generateMultipartFormData(args, bondary);
        }
        result << "Content-Length: " << requestData.length() << detail::CRLF
               << detail::CRLF;
        result << requestData;
    }
    return result.str();
}

std::string HttpParser::generateMultipartFormData(const HttpReqArg::Vec& args,
                                                  const std::string& boundary) {
    std::stringstream result;
    for (const auto& item : args) {
        result << item->create_mime_part(boundary);
    }
    result << "--" + boundary + "--" << detail::CRLF;
    return result.str();
}

std::string HttpParser::generateMultipartBoundary(const HttpReqArg::Vec& args) {
    std::string result;
    for (const auto& item : args) {
        if (item->isFile()) {
            while (result.empty() ||
                   item->value.find(result) != std::string::npos) {
                result += StringTools::generateRandomString(4);
            }
        }
    }
    return result;
}

std::string HttpParser::generateWwwFormUrlencoded(const HttpReqArg::Vec& args) {
    std::string result;

    bool firstRun = true;
    for (const auto& item : args) {
        if (firstRun) {
            firstRun = false;
        } else {
            result += '&';
        }
        result += StringTools::urlEncode(item->name);
        result += '=';
        result += StringTools::urlEncode(item->value);
    }

    return result;
}

std::string HttpParser::generateResponse(const std::string& data,
                                         const std::string& mimeType,
                                         unsigned short statusCode,
                                         const std::string& statusStr,
                                         bool isKeepAlive) {
    std::stringstream result;
    result << "HTTP/1.1 " << statusCode << " " << statusStr << detail::CRLF;
    result << "Content-Type: " << mimeType << detail::CRLF;
    result << "Content-Length: " << data.length() << detail::CRLF;
    result << "Connection: " << (isKeepAlive ? "keep-alive" : "close");
    result << detail::CRLF << detail::CRLF;
    result << data;
    return result.str();
}

std::unordered_map<std::string, std::string> HttpParser::parseHeader(
    const std::string& data, bool isRequest) {
    std::unordered_map<std::string, std::string> headers;
    std::istringstream stream(data);
    std::string line;

    // Read the first line (Request line or Status line)
    if (!std::getline(stream, line)) {
        // Handle error: No data
        return headers;
    }

    // Remove trailing '\r' if present
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    std::istringstream lineStream(line);
    std::string arg1, arg2, arg3;

    if (!(lineStream >> arg1 >> arg2 >> arg3)) {
        // Handle error: Malformed first line
        std::cerr << "Cannot parse line " << line << std::endl;
        return headers;
    }

    if (isRequest) {
        // Parse Request Line: METHOD SP REQUEST-URI SP HTTP-VERSION
        headers["_method"] = arg1;
        headers["_path"] = arg2;
    } else {
        // Parse Status Line: HTTP-VERSION SP STATUS-CODE SP REASON-PHRASE
        headers["_status"] = arg2;
    }

    // Read header lines
    while (std::getline(stream, line)) {
        // Remove trailing '\r' if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        // Empty line indicates the end of headers
        if (line.empty()) {
            break;
        }

        // Find the colon separating the header name and value
        auto colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            // Handle error: Malformed header line
            continue;  // Skip invalid header
        }

        std::string headerName = line.substr(0, colonPos);
        std::string headerValue = line.substr(colonPos + 1);

        // Trim whitespace from header name and value
        boost::trim(headerName);
        boost::trim(headerValue);

        headers[headerName] = headerValue;
    }

    return headers;
}

std::string HttpParser::extractBody(const std::string& data) {
    std::size_t headerEnd = data.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        return data;
    }
    headerEnd += 4;
    return data.substr(headerEnd);
}

}  // namespace TgBot
