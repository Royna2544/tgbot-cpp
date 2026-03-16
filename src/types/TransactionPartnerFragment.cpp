#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(TransactionPartnerFragment) {
    auto result = std::make_shared<TransactionPartnerFragment>();
    parse(data, "type", &result->type);
    result->withdrawalState = parse<RevenueWithdrawalState>(data, "withdrawal_state");
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerFragment) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("withdrawal_state", object->withdrawalState);
    }
    return json;
}

} // namespace TgBot
