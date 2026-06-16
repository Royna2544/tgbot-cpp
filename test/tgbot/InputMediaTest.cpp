#include <boost/test/unit_test.hpp>

#include <memory>

#include <nlohmann/json.hpp>
#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMedia.h>
#include <tgbot/types/InputMediaLink.h>
#include <tgbot/types/InputMediaLocation.h>
#include <tgbot/types/InputMediaPhoto.h>

using namespace TgBot;

BOOST_AUTO_TEST_SUITE(tInputMedia)

// The three spec bases InputMedia / InputPollMedia / InputPollOptionMedia are
// collapsed into the single base InputMedia, whose dispatcher covers the union
// of all leaves. These cases exercise a leaf that belongs to all three
// (photo), a poll-only leaf (location), and an option-only leaf (link).

BOOST_AUTO_TEST_CASE(fileLeafRoundTrip) {
    const auto in = nlohmann::json::parse(
        R"({"type":"photo","media":"attach://p","has_spoiler":true})");
    auto m = parse<InputMedia>(in);
    BOOST_REQUIRE(m != nullptr);
    BOOST_CHECK_EQUAL(m->type, "photo");
    BOOST_CHECK_EQUAL(std::static_pointer_cast<InputMediaPhoto>(m)->media,
                      "attach://p");
    BOOST_CHECK(put(m) == in);
}

BOOST_AUTO_TEST_CASE(pollLeafRoundTrip) {
    const auto in = nlohmann::json::parse(
        R"({"type":"location","latitude":1.5,"longitude":2.5})");
    auto m = parse<InputMedia>(in);
    BOOST_REQUIRE(m != nullptr);
    BOOST_CHECK_EQUAL(m->type, "location");
    BOOST_CHECK_EQUAL(std::static_pointer_cast<InputMediaLocation>(m)->latitude,
                      1.5f);
    BOOST_CHECK(put(m) == in);
}

BOOST_AUTO_TEST_CASE(optionLeafRoundTrip) {
    const auto in = nlohmann::json::parse(R"({"type":"link","url":"http://x"})");
    auto m = parse<InputMedia>(in);
    BOOST_REQUIRE(m != nullptr);
    BOOST_CHECK_EQUAL(m->type, "link");
    BOOST_CHECK_EQUAL(std::static_pointer_cast<InputMediaLink>(m)->url,
                      "http://x");
    BOOST_CHECK(put(m) == in);
}

BOOST_AUTO_TEST_SUITE_END()
