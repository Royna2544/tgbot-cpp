#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(LabeledPrice) {
    auto result = std::make_shared<LabeledPrice>();
    parse(data, "label", &result->label);
    parse(data, "amount", &result->amount);
    return result;
}

DECLARE_PARSER_TO_JSON(LabeledPrice) {
    JsonWrapper json;
    if (object) {
        json.put("label", object->label);
        json.put("amount", object->amount);
    }
    return json;
}

} // namespace TgBot
