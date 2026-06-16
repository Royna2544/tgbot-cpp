#include <tgbot/TgTypeParser.h>
#include <tgbot/types/TransactionPartnerFragment.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<TransactionPartnerFragment> parse(const nlohmann::json &data) {
    auto result = std::make_shared<TransactionPartnerFragment>();
    parse(data, "type", &result->type);
    result->withdrawalState = parse<RevenueWithdrawalState>(data, "withdrawal_state");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<TransactionPartnerFragment> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("withdrawal_state", object->withdrawalState);
    }
    return json;
}

} // namespace TgBot
