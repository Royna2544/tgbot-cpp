#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichText.h>
#include <tgbot/types/RichTextString.h>
#include <tgbot/types/RichTextArray.h>
#include <tgbot/types/RichTextBold.h>
#include <tgbot/types/RichTextItalic.h>
#include <tgbot/types/RichTextUnderline.h>
#include <tgbot/types/RichTextStrikethrough.h>
#include <tgbot/types/RichTextSpoiler.h>
#include <tgbot/types/RichTextDateTime.h>
#include <tgbot/types/RichTextTextMention.h>
#include <tgbot/types/RichTextSubscript.h>
#include <tgbot/types/RichTextSuperscript.h>
#include <tgbot/types/RichTextMarked.h>
#include <tgbot/types/RichTextCode.h>
#include <tgbot/types/RichTextCustomEmoji.h>
#include <tgbot/types/RichTextMathematicalExpression.h>
#include <tgbot/types/RichTextUrl.h>
#include <tgbot/types/RichTextEmailAddress.h>
#include <tgbot/types/RichTextPhoneNumber.h>
#include <tgbot/types/RichTextBankCardNumber.h>
#include <tgbot/types/RichTextMention.h>
#include <tgbot/types/RichTextHashtag.h>
#include <tgbot/types/RichTextCashtag.h>
#include <tgbot/types/RichTextBotCommand.h>
#include <tgbot/types/RichTextAnchor.h>
#include <tgbot/types/RichTextAnchorLink.h>
#include <tgbot/types/RichTextReference.h>
#include <tgbot/types/RichTextReferenceLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichText) {
    if (data.is_string()) {
        auto result = std::make_shared<RichTextString>();
        result->text = data.get<std::string>();
        return result;
    }
    if (data.is_array()) {
        auto result = std::make_shared<RichTextArray>();
        result->items = parseArray<RichText>(data);
        return result;
    }
    std::string type;
    parse(data, "type", &type);
    if (type == "bold") {
        return parse<RichTextBold>(data);
    } else if (type == "italic") {
        return parse<RichTextItalic>(data);
    } else if (type == "underline") {
        return parse<RichTextUnderline>(data);
    } else if (type == "strikethrough") {
        return parse<RichTextStrikethrough>(data);
    } else if (type == "spoiler") {
        return parse<RichTextSpoiler>(data);
    } else if (type == "date_time") {
        return parse<RichTextDateTime>(data);
    } else if (type == "text_mention") {
        return parse<RichTextTextMention>(data);
    } else if (type == "subscript") {
        return parse<RichTextSubscript>(data);
    } else if (type == "superscript") {
        return parse<RichTextSuperscript>(data);
    } else if (type == "marked") {
        return parse<RichTextMarked>(data);
    } else if (type == "code") {
        return parse<RichTextCode>(data);
    } else if (type == "custom_emoji") {
        return parse<RichTextCustomEmoji>(data);
    } else if (type == "mathematical_expression") {
        return parse<RichTextMathematicalExpression>(data);
    } else if (type == "url") {
        return parse<RichTextUrl>(data);
    } else if (type == "email_address") {
        return parse<RichTextEmailAddress>(data);
    } else if (type == "phone_number") {
        return parse<RichTextPhoneNumber>(data);
    } else if (type == "bank_card_number") {
        return parse<RichTextBankCardNumber>(data);
    } else if (type == "mention") {
        return parse<RichTextMention>(data);
    } else if (type == "hashtag") {
        return parse<RichTextHashtag>(data);
    } else if (type == "cashtag") {
        return parse<RichTextCashtag>(data);
    } else if (type == "bot_command") {
        return parse<RichTextBotCommand>(data);
    } else if (type == "anchor") {
        return parse<RichTextAnchor>(data);
    } else if (type == "anchor_link") {
        return parse<RichTextAnchorLink>(data);
    } else if (type == "reference") {
        return parse<RichTextReference>(data);
    } else if (type == "reference_link") {
        return parse<RichTextReferenceLink>(data);
    } else {
        throw invalidType("RichText", type);
    }
}

DECLARE_PARSER_TO_JSON(RichText) {
    if (!object) {
        return {};
    }
    if (object->kind == RichText::Kind::String) {
        return std::static_pointer_cast<RichTextString>(object)->text;
    }
    if (object->kind == RichText::Kind::Array) {
        return put(std::static_pointer_cast<RichTextArray>(object)->items);
    }
    JsonWrapper json;
    json.put("type", object->type);
    if (object->type == "bold") {
        json += put<RichTextBold>(object);
    } else if (object->type == "italic") {
        json += put<RichTextItalic>(object);
    } else if (object->type == "underline") {
        json += put<RichTextUnderline>(object);
    } else if (object->type == "strikethrough") {
        json += put<RichTextStrikethrough>(object);
    } else if (object->type == "spoiler") {
        json += put<RichTextSpoiler>(object);
    } else if (object->type == "date_time") {
        json += put<RichTextDateTime>(object);
    } else if (object->type == "text_mention") {
        json += put<RichTextTextMention>(object);
    } else if (object->type == "subscript") {
        json += put<RichTextSubscript>(object);
    } else if (object->type == "superscript") {
        json += put<RichTextSuperscript>(object);
    } else if (object->type == "marked") {
        json += put<RichTextMarked>(object);
    } else if (object->type == "code") {
        json += put<RichTextCode>(object);
    } else if (object->type == "custom_emoji") {
        json += put<RichTextCustomEmoji>(object);
    } else if (object->type == "mathematical_expression") {
        json += put<RichTextMathematicalExpression>(object);
    } else if (object->type == "url") {
        json += put<RichTextUrl>(object);
    } else if (object->type == "email_address") {
        json += put<RichTextEmailAddress>(object);
    } else if (object->type == "phone_number") {
        json += put<RichTextPhoneNumber>(object);
    } else if (object->type == "bank_card_number") {
        json += put<RichTextBankCardNumber>(object);
    } else if (object->type == "mention") {
        json += put<RichTextMention>(object);
    } else if (object->type == "hashtag") {
        json += put<RichTextHashtag>(object);
    } else if (object->type == "cashtag") {
        json += put<RichTextCashtag>(object);
    } else if (object->type == "bot_command") {
        json += put<RichTextBotCommand>(object);
    } else if (object->type == "anchor") {
        json += put<RichTextAnchor>(object);
    } else if (object->type == "anchor_link") {
        json += put<RichTextAnchorLink>(object);
    } else if (object->type == "reference") {
        json += put<RichTextReference>(object);
    } else if (object->type == "reference_link") {
        json += put<RichTextReferenceLink>(object);
    } else {
        throw invalidType("RichText", object->type);
    }
    return json;
}

} // namespace TgBot
