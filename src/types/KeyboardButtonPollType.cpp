#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(KeyboardButtonPollType) {
    auto result = std::make_shared<KeyboardButtonPollType>();
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonPollType) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
