#include <tgbot/TgTypeParser.h>
#include <tgbot/types/TransactionPartnerAffiliateProgram.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<TransactionPartnerAffiliateProgram> parse(const nlohmann::json &data) {
    auto result = std::make_shared<TransactionPartnerAffiliateProgram>();
    parse(data, "type", &result->type);
    result->sponsorUser = parse<User>(data, "sponsor_user");
    parse(data, "commission_per_mille", &result->commissionPerMille);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<TransactionPartnerAffiliateProgram> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("sponsor_user", object->sponsorUser);
        json.put("commission_per_mille", object->commissionPerMille);
    }
    return json;
}

} // namespace TgBot
