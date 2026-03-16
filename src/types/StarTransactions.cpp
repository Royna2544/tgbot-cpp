#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StarTransactions) {
    auto result = std::make_shared<StarTransactions>();
    result->transactions = parseRequiredArray<StarTransaction>(data, "transactions");
    return result;
}

DECLARE_PARSER_TO_JSON(StarTransactions) {
    JsonWrapper json;
    if (object) {
        json.put("transactions", object->transactions);
    }
    return json;
}

} // namespace TgBot
