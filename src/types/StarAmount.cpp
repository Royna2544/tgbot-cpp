#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StarAmount) {
    auto result = std::make_shared<StarAmount>();
    parse(data, "amount", &result->amount);
    parse(data, "nanostar_amount", &result->nanostarAmount);
    return result;
}

DECLARE_PARSER_TO_JSON(StarAmount) {
    JsonWrapper json;
    if (object) {
        json.put("amount", object->amount);
        json.put("nanostar_amount", object->nanostarAmount);
    }
    return json;
}

} // namespace TgBot
