#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotCommand) {
    auto result = std::make_shared<BotCommand>();
    parse(data, "command", &result->command);
    parse(data, "description", &result->description);
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommand) {
    JsonWrapper json;
    if (object) {
        json.put("command", object->command);
        json.put("description", object->description);
    }
    return json;
}

} // namespace TgBot
