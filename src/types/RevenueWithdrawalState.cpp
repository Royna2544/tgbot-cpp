#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RevenueWithdrawalState.h>
#include <tgbot/types/RevenueWithdrawalStateFailed.h>
#include <tgbot/types/RevenueWithdrawalStatePending.h>
#include <tgbot/types/RevenueWithdrawalStateSucceeded.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RevenueWithdrawalState> parse(const nlohmann::json &data) {
    std::string type;
    RevenueWithdrawalState::Ptr result;
    parse(data, "type", &type);
    if (type == "pending") {
        result = parse<RevenueWithdrawalStatePending>(data);
    } else if (type == "succeeded") {
        result = parse<RevenueWithdrawalStateSucceeded>(data);
    } else if (type == "failed") {
        result = parse<RevenueWithdrawalStateFailed>(data);
    } else {
        throw invalidType("RevenueWithdrawalState", type);
    }

    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RevenueWithdrawalState> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "pending") {
            json += put<RevenueWithdrawalStatePending>(object);
        } else if (object->type == "succeeded") {
            json += put<RevenueWithdrawalStateSucceeded>(object);
        } else if (object->type == "failed") {
            json += put<RevenueWithdrawalStateFailed>(object);
        } else {
            throw invalidType("RevenueWithdrawalState", object->type);
        }
    }
    return json;
}

} // namespace TgBot
