#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GiveawayCompleted) {
    auto result = std::make_shared<GiveawayCompleted>();
    parse(data, "winner_count", &result->winnerCount);
    parse(data, "unclaimed_prize_count", &result->unclaimedPrizeCount);
    result->giveawayMessage = parse<Message>(data, "giveaway_message");
    parse(data, "is_star_giveaway", &result->isStarGiveaway);
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayCompleted) {
    JsonWrapper json;
    if (object) {
        json.put("winner_count", object->winnerCount);
        json.put("unclaimed_prize_count", object->unclaimedPrizeCount);
        json.put("giveaway_message", object->giveawayMessage);
        json.put("is_star_giveaway", object->isStarGiveaway);
    }
    return json;
}

} // namespace TgBot
