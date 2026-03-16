#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GameHighScore) {
    auto result = std::make_shared<GameHighScore>();
    parse(data, "position", &result->position);
    result->user = parseRequired<User>(data, "user");
    parse(data, "score", &result->score);
    return result;
}

DECLARE_PARSER_TO_JSON(GameHighScore) {
    JsonWrapper json;
    if (object) {
        json.put("position", object->position);
        json.put("user", object->user);
        json.put("score", object->score);
    }
    return json;
}

} // namespace TgBot
