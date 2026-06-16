#include <tgbot/TgTypeParser.h>
#include <tgbot/types/GiveawayCreated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<GiveawayCreated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<GiveawayCreated>();
    parse(data, "prize_star_count", &result->prizeStarCount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<GiveawayCreated> &object) {
    JsonWrapper json;
    if (object) {
        json.put("prize_star_count", object->prizeStarCount);
    }
    return json;
}

} // namespace TgBot
