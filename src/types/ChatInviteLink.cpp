#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatInviteLink) {
    auto result = std::make_shared<ChatInviteLink>();
    parse(data, "invite_link", &result->inviteLink);
    result->creator = parseRequired<User>(data, "creator");
    parse(data, "creates_join_request", &result->createsJoinRequest);
    parse(data, "is_primary", &result->isPrimary);
    parse(data, "is_revoked", &result->isRevoked);
    parse(data, "name", &result->name);
    parse(data, "expire_date", &result->expireDate);
    parse(data, "member_limit", &result->memberLimit);
    parse(data, "pending_join_request_count", &result->pendingJoinRequestCount);
    parse(data, "subscription_period", &result->subscriptionPeriod);
    parse(data, "subscription_price", &result->subscriptionPrice);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatInviteLink) {
    JsonWrapper json;
    if (object) {
        json.put("invite_link", object->inviteLink);
        json.put("creator", object->creator);
        json.put("creates_join_request", object->createsJoinRequest);
        json.put("is_primary", object->isPrimary);
        json.put("is_revoked", object->isRevoked);
        json.put("name", object->name);
        json.put("expire_date", object->expireDate);
        json.put("member_limit", object->memberLimit);
        json.put("pending_join_request_count", object->pendingJoinRequestCount);
        json.put("subscription_period", object->subscriptionPeriod);
        json.put("subscription_price", object->subscriptionPrice);
    }
    return json;
}

} // namespace TgBot
