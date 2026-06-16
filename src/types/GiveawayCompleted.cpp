#include <tgbot/TgTypeParser.h>
#include <tgbot/types/GiveawayCompleted.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<GiveawayCompleted> parse(const nlohmann::json &data) {
    auto result = std::make_shared<GiveawayCompleted>();
    parse(data, "winner_count", &result->winnerCount);
    parse(data, "unclaimed_prize_count", &result->unclaimedPrizeCount);
    result->giveawayMessage = parse<Message>(data, "giveaway_message");
    parse(data, "is_star_giveaway", &result->isStarGiveaway);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<GiveawayCompleted> &object) {
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
