#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RevenueWithdrawalState) {
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

DECLARE_PARSER_TO_JSON(RevenueWithdrawalState) {
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
