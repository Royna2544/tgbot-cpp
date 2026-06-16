#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatBoostSourceGiveaway.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatBoostSourceGiveaway> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatBoostSourceGiveaway>();
    parse(data, "source", &result->source);
    parse(data, "giveaway_message_id", &result->giveawayMessageId);
    result->user = parseRequired<User>(data, "user");
    parse(data, "prize_star_count", &result->prizeStarCount);
    parse(data, "is_unclaimed", &result->isUnclaimed);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatBoostSourceGiveaway> &object) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        json.put("giveaway_message_id", object->giveawayMessageId);
        json.put("user", object->user);
        json.put("prize_star_count", object->prizeStarCount);
        json.put("is_unclaimed", object->isUnclaimed);
    }
    return json;
}

} // namespace TgBot
