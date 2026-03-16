#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GiveawayWinners) {
    auto result = std::make_shared<GiveawayWinners>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "giveaway_message_id", &result->giveawayMessageId);
    parse(data, "winners_selection_date", &result->winnersSelectionDate);
    parse(data, "winner_count", &result->winnerCount);
    result->winners = parseRequiredArray<User>(data, "winners");
    parse(data, "additional_chat_count", &result->additionalChatCount);
    parse(data, "prize_star_count", &result->prizeStarCount);
    parse(data, "premium_subscription_month_count", &result->premiumSubscriptionMonthCount);
    parse(data, "unclaimed_prize_count", &result->unclaimedPrizeCount);
    parse(data, "only_new_members", &result->onlyNewMembers);
    parse(data, "was_refunded", &result->wasRefunded);
    parse(data, "prize_description", &result->prizeDescription);
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayWinners) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("giveaway_message_id", object->giveawayMessageId);
        json.put("winners_selection_date", object->winnersSelectionDate);
        json.put("winner_count", object->winnerCount);
        json.put("winners", object->winners);
        json.put("additional_chat_count", object->additionalChatCount);
        json.put("prize_star_count", object->prizeStarCount);
        json.put("premium_subscription_month_count", object->premiumSubscriptionMonthCount);
        json.put("unclaimed_prize_count", object->unclaimedPrizeCount);
        json.put("only_new_members", object->onlyNewMembers);
        json.put("was_refunded", object->wasRefunded);
        json.put("prize_description", object->prizeDescription);
    }
    return json;
}

} // namespace TgBot
