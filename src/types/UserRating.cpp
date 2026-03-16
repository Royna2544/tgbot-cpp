#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UserRating) {
    auto result = std::make_shared<UserRating>();
    parse(data, "level", &result->level);
    parse(data, "rating", &result->rating);
    parse(data, "current_level_rating", &result->currentLevelRating);
    parse(data, "next_level_rating", &result->nextLevelRating);
    return result;
}

DECLARE_PARSER_TO_JSON(UserRating) {
    JsonWrapper json;
    if (object) {
        json.put("level", object->level);
        json.put("rating", object->rating);
        json.put("current_level_rating", object->currentLevelRating);
        json.put("next_level_rating", object->nextLevelRating);
    }
    return json;
}

} // namespace TgBot
