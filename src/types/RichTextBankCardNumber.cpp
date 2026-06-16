#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextBankCardNumber.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextBankCardNumber> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextBankCardNumber>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "bank_card_number", &result->bankCardNumber);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextBankCardNumber> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("bank_card_number", object->bankCardNumber);
    }
    return json;
}

} // namespace TgBot
