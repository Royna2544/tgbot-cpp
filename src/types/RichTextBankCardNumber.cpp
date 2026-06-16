#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextBankCardNumber.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextBankCardNumber) {
    auto result = std::make_shared<RichTextBankCardNumber>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "bank_card_number", &result->bankCardNumber);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextBankCardNumber) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("bank_card_number", object->bankCardNumber);
    }
    return json;
}

} // namespace TgBot
