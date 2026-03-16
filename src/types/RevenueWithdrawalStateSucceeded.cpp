#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RevenueWithdrawalStateSucceeded) {
    auto result = std::make_shared<RevenueWithdrawalStateSucceeded>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    parse(data, "url", &result->url);
    return result;
}

DECLARE_PARSER_TO_JSON(RevenueWithdrawalStateSucceeded) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
