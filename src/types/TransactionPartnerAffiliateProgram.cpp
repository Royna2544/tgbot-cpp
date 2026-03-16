#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(TransactionPartnerAffiliateProgram) {
    auto result = std::make_shared<TransactionPartnerAffiliateProgram>();
    parse(data, "type", &result->type);
    result->sponsorUser = parse<User>(data, "sponsor_user");
    parse(data, "commission_per_mille", &result->commissionPerMille);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerAffiliateProgram) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("sponsor_user", object->sponsorUser);
        json.put("commission_per_mille", object->commissionPerMille);
    }
    return json;
}

} // namespace TgBot
