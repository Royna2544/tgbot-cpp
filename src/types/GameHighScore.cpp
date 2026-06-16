#include <tgbot/TgTypeParser.h>
#include <tgbot/types/GameHighScore.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<GameHighScore> parse(const nlohmann::json &data) {
    auto result = std::make_shared<GameHighScore>();
    parse(data, "position", &result->position);
    result->user = parseRequired<User>(data, "user");
    parse(data, "score", &result->score);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<GameHighScore> &object) {
    JsonWrapper json;
    if (object) {
        json.put("position", object->position);
        json.put("user", object->user);
        json.put("score", object->score);
    }
    return json;
}

} // namespace TgBot
