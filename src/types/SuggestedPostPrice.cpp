#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostPrice) {
    auto result = std::make_shared<SuggestedPostPrice>();
    parse(data, "currency", &result->currency);
    parse(data, "amount", &result->amount);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostPrice) {
    JsonWrapper json;
    if (object) {
        json.put("currency", object->currency);
        json.put("amount", object->amount);
    }
    return json;
}

} // namespace TgBot
