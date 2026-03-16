#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(TransactionPartnerUser) {
    auto result = std::make_shared<TransactionPartnerUser>();
    parse(data, "type", &result->type);
    parse(data, "transaction_type", &result->transactionType);
    result->user = parseRequired<User>(data, "user");
    result->affiliate = parse<AffiliateInfo>(data, "affiliate");
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "subscription_period", &result->subscriptionPeriod);
    result->paidMedia = parseArray<PaidMedia>(data, "paid_media");
    parse(data, "paid_media_payload", &result->paidMediaPayload);
    result->gift = parse<Gift>(data, "gift");
    parse(data, "premium_subscription_duration", &result->premiumSubscriptionDuration);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerUser) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("transaction_type", object->transactionType);
        json.put("user", object->user);
        json.put("affiliate", object->affiliate);
        json.put("invoice_payload", object->invoicePayload);
        json.put("subscription_period", object->subscriptionPeriod);
        json.put("paid_media", object->paidMedia);
        json.put("paid_media_payload", object->paidMediaPayload);
        json.put("gift", object->gift);
        json.put("premium_subscription_duration", object->premiumSubscriptionDuration);
    }
    return json;
}

} // namespace TgBot
