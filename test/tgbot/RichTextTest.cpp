#include <boost/test/unit_test.hpp>

#include <memory>

#include <nlohmann/json.hpp>
#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlock.h>
#include <tgbot/types/RichBlockBlockQuotation.h>
#include <tgbot/types/RichBlockParagraph.h>
#include <tgbot/types/RichText.h>
#include <tgbot/types/RichTextArray.h>
#include <tgbot/types/RichTextBold.h>
#include <tgbot/types/RichTextString.h>
#include <tgbot/types/RichTextUrl.h>

using namespace TgBot;

BOOST_AUTO_TEST_SUITE(tRichText)

// A bold node whose text is a sequence of a plain string and a url node -- it
// exercises all three RichText forms (Object / Array / String) and recursion.
BOOST_AUTO_TEST_CASE(recursiveRoundTrip) {
    const auto in = nlohmann::json::parse(
        R"({"type":"bold","text":["hello ",)"
        R"({"type":"url","text":"link","url":"http://x"}]})");

    auto rt = parse<RichText>(in);

    BOOST_REQUIRE(rt != nullptr);
    BOOST_CHECK(rt->kind == RichText::Kind::Object);
    BOOST_CHECK_EQUAL(rt->type, "bold");
    auto bold = std::static_pointer_cast<RichTextBold>(rt);
    BOOST_REQUIRE(bold->text != nullptr);
    BOOST_CHECK(bold->text->kind == RichText::Kind::Array);
    auto arr = std::static_pointer_cast<RichTextArray>(bold->text);
    BOOST_REQUIRE_EQUAL(arr->items.size(), 2u);
    BOOST_CHECK(arr->items[0]->kind == RichText::Kind::String);
    BOOST_CHECK_EQUAL(
        std::static_pointer_cast<RichTextString>(arr->items[0])->text, "hello ");
    BOOST_CHECK_EQUAL(arr->items[1]->type, "url");

    // Serialising back reproduces the input exactly (nlohmann == ignores object
    // key order; array order is preserved).
    BOOST_CHECK(put(rt) == in);
}

// A blockquote containing a paragraph (block recursion) whose text and credit
// are RichText -- exercises RichBlock dispatch + nested blocks + RichText.
BOOST_AUTO_TEST_CASE(richBlockRoundTrip) {
    const auto in = nlohmann::json::parse(
        R"({"type":"blockquote",)"
        R"("blocks":[{"type":"paragraph","text":"hi"}],"credit":"src"})");

    auto rb = parse<RichBlock>(in);

    BOOST_REQUIRE(rb != nullptr);
    BOOST_CHECK_EQUAL(rb->type, "blockquote");
    auto quote = std::static_pointer_cast<RichBlockBlockQuotation>(rb);
    BOOST_REQUIRE_EQUAL(quote->blocks.size(), 1u);
    BOOST_CHECK_EQUAL(quote->blocks[0]->type, "paragraph");
    BOOST_CHECK(put(rb) == in);
}

BOOST_AUTO_TEST_SUITE_END()
