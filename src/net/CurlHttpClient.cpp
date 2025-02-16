
#include <curl/curl.h>
#include <curl/easy.h>

#include <chrono>

#include "tgbot/TgException.h"
#include "tgbot/net/HttpClient.h"
#ifdef HAVE_CURL

#include <array>
#include <cstddef>
#include <cstring>
#include <string>

#include "tgbot/net/CurlHttpClient.h"

namespace TgBot {

CurlHttpClient::CurlHttpClient(std::chrono::seconds timeout)
    : HttpClient(timeout) {}

CurlHttpClient::~CurlHttpClient() {
    std::lock_guard<std::mutex> lock(curlHandlesMutex);
    for (auto& c : curlHandles) {
        curl_easy_cleanup(c.second);
    }
}

static std::size_t curlWriteString(char* ptr, std::size_t size,
                                   std::size_t nmemb, void* userdata) {
    static_cast<std::string*>(userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string CurlHttpClient::makeRequest(const Url& url,

                                        const HttpReqArg::Vec& args) const {
    CURL* curl = nullptr;
    {
        std::lock_guard<std::mutex> lock(curlHandlesMutex);
        auto id = std::this_thread::get_id();
        auto it = curlHandles.find(id);
        if (it == curlHandles.end()) {
            curl = curl_easy_init();
            if (!curl) {
                throw NetworkException(NetworkException::State::Unknown,
                                       "curl_easy_init() failed");
            }
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout().count());
            curlHandles[id] = curl;
        } else
            curl = it->second;
    }

    std::string u = url.protocol + "://" + url.host + url.path;
    if (args.empty()) {
        u += "?" + url.query;
    }
    curl_easy_setopt(curl, CURLOPT_URL, u.c_str());

    curl_mime* mime = nullptr;
    mime = curl_mime_init(curl);
    if (!args.empty()) {
        for (const auto& a : args) {
            a->update_mime_part(curl_mime_addpart(mime));
        }
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteString);

    std::array<char, CURL_ERROR_SIZE> errbuf{};
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf.data());

    if (auto cert = getServerCert(); cert) {
        curl_easy_setopt(curl, CURLOPT_CERTINFO, cert->string().c_str());
    }

    auto res = curl_easy_perform(curl);
    curl_mime_free(mime);

    // If the request did not complete correctly, show the error
    // information. If no detailed error information was written to errbuf
    // show the more generic information from curl_easy_strerror instead.
    if (res != CURLE_OK) {
        size_t len = strlen(errbuf.data());
        std::string errmsg;
        if (len != 0) {
            errmsg = errbuf.data();
        } else {
            errmsg = curl_easy_strerror(res);
        }
        throw NetworkException(NetworkException::State::Unknown,
                               std::string("cURL error: ") + errmsg);
    }

    return HttpParser::extractBody(response);
}

}  // namespace TgBot

#endif
