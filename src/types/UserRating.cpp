#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UserRating.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UserRating> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UserRating>();
    parse(data, "level", &result->level);
    parse(data, "rating", &result->rating);
    parse(data, "current_level_rating", &result->currentLevelRating);
    parse(data, "next_level_rating", &result->nextLevelRating);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UserRating> &object) {
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
