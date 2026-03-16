#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(AffiliateInfo) {
    auto result = std::make_shared<AffiliateInfo>();
    result->affiliateUser = parse<User>(data, "affiliate_user");
    result->affiliateChat = parse<Chat>(data, "affiliate_chat");
    parse(data, "commission_per_mille", &result->commissionPerMille);
    parse(data, "amount", &result->amount);
    parse(data, "nanostar_amount", &result->nanostarAmount);
    return result;
}

DECLARE_PARSER_TO_JSON(AffiliateInfo) {
    JsonWrapper json;
    if (object) {
        json.put("affiliate_user", object->affiliateUser);
        json.put("affiliate_chat", object->affiliateChat);
        json.put("commission_per_mille", object->commissionPerMille);
        json.put("amount", object->amount);
        json.put("nanostar_amount", object->nanostarAmount);
    }
    return json;
}

} // namespace TgBot
