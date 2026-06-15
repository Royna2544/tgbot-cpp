#include <boost/test/unit_test.hpp>

#include <chrono>
#include <cstdint>
#include <map>
#include <string>

#include <tgbot/Api.h>
#include <tgbot/TgException.h>
#include <tgbot/net/HttpClient.h>
#include <tgbot/net/HttpReqArg.h>
#include <tgbot/net/Url.h>
#include <tgbot/tools/StringTools.h>

using namespace TgBot;

namespace {

// HttpClient stub that records the outgoing request and returns a canned
// response, so Api methods can be exercised end-to-end without a network.
class MockHttpClient : public HttpClient {
   public:
    MockHttpClient() : HttpClient(std::chrono::seconds(1)) {}

    std::string response;
    mutable int callCount = 0;
    mutable std::string lastPath;
    mutable std::map<std::string, std::string> lastArgs;

    std::string makeRequest(const Url& url,
                            const HttpReqArg::Vec& args) const override {
        ++callCount;
        lastPath = url.path;
        lastArgs.clear();
        for (const auto& arg : args) {
            lastArgs[arg->name] = arg->value;
        }
        return response;
    }
};

}  // namespace

BOOST_AUTO_TEST_SUITE(tApi)

BOOST_AUTO_TEST_CASE(getMe_parsesResult) {
    MockHttpClient http;
    http.response =
        R"({"ok":true,"result":{"id":42,"is_bot":true,)"
        R"("first_name":"TestBot","username":"test_bot"}})";
    Api api("TOKEN", &http, "https://api.telegram.org");

    auto user = api.getMe();

    BOOST_CHECK_EQUAL(http.callCount, 1);
    BOOST_CHECK(StringTools::endsWith(http.lastPath, "/getMe"));
    BOOST_REQUIRE(user != nullptr);
    BOOST_CHECK_EQUAL(user->id, 42);
    BOOST_CHECK(user->isBot);
    BOOST_CHECK_EQUAL(user->firstName, "TestBot");
    BOOST_REQUIRE(user->username.has_value());
    BOOST_CHECK_EQUAL(*user->username, "test_bot");
}

BOOST_AUTO_TEST_CASE(sendMessage_serializesArgsAndParsesResult) {
    MockHttpClient http;
    http.response =
        R"({"ok":true,"result":{"message_id":99,"date":1,)"
        R"("chat":{"id":12345,"type":"private"},"text":"Hello"}})";
    Api api("TOKEN", &http, "https://api.telegram.org");

    auto msg = api.sendMessage(std::int64_t{12345}, "Hello");

    BOOST_CHECK(StringTools::endsWith(http.lastPath, "/sendMessage"));
    BOOST_CHECK_EQUAL(http.lastArgs["chat_id"], "12345");
    BOOST_CHECK_EQUAL(http.lastArgs["text"], "Hello");
    BOOST_REQUIRE(msg != nullptr);
    BOOST_CHECK_EQUAL(msg->messageId, 99);
    BOOST_REQUIRE(msg->chat != nullptr);
    BOOST_CHECK_EQUAL(msg->chat->id, 12345);
    BOOST_REQUIRE(msg->text.has_value());
    BOOST_CHECK_EQUAL(*msg->text, "Hello");
}

BOOST_AUTO_TEST_CASE(apiError_throwsWithoutRetry) {
    MockHttpClient http;
    http.response =
        R"({"ok":false,"error_code":400,)"
        R"("description":"Bad Request: chat not found"})";
    Api api("TOKEN", &http, "https://api.telegram.org");

    BOOST_CHECK_THROW(api.getMe(), TgException);
    // A deterministic API error must not be retried.
    BOOST_CHECK_EQUAL(http.callCount, 1);
}

BOOST_AUTO_TEST_SUITE_END()
