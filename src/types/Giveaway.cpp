#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Giveaway) {
    auto result = std::make_shared<Giveaway>();
    result->chats = parseRequiredArray<Chat>(data, "chats");
    parse(data, "winners_selection_date", &result->winnersSelectionDate);
    parse(data, "winner_count", &result->winnerCount);
    parse(data, "only_new_members", &result->onlyNewMembers);
    parse(data, "has_public_winners", &result->hasPublicWinners);
    parse(data, "prize_description", &result->prizeDescription);
    result->countryCodes = parsePrimitiveArray<std::string>(data, "country_codes");
    parse(data, "prize_star_count", &result->prizeStarCount);
    parse(data, "premium_subscription_month_count", &result->premiumSubscriptionMonthCount);
    return result;
}

DECLARE_PARSER_TO_JSON(Giveaway) {
    JsonWrapper json;
    if (object) {
        json.put("chats", object->chats);
        json.put("winners_selection_date", object->winnersSelectionDate);
        json.put("winner_count", object->winnerCount);
        json.put("only_new_members", object->onlyNewMembers);
        json.put("has_public_winners", object->hasPublicWinners);
        json.put("prize_description", object->prizeDescription);
        json.put("country_codes", object->countryCodes);
        json.put("prize_star_count", object->prizeStarCount);
        json.put("premium_subscription_month_count", object->premiumSubscriptionMonthCount);
    }
    return json;
}

} // namespace TgBot
