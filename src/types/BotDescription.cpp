#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotDescription) {
    auto result = std::make_shared<BotDescription>();
    parse(data, "description", &result->description);
    return result;
}

DECLARE_PARSER_TO_JSON(BotDescription) {
    JsonWrapper json;
    if (object) {
        json.put("description", object->description);
    }
    return json;
}

} // namespace TgBot
