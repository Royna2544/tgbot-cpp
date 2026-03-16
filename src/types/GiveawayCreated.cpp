#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GiveawayCreated) {
    auto result = std::make_shared<GiveawayCreated>();
    parse(data, "prize_star_count", &result->prizeStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(GiveawayCreated) {
    JsonWrapper json;
    if (object) {
        json.put("prize_star_count", object->prizeStarCount);
    }
    return json;
}

} // namespace TgBot
