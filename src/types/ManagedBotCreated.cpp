#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ManagedBotCreated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ManagedBotCreated) {
    auto result = std::make_shared<ManagedBotCreated>();
    result->bot = parseRequired<User>(data, "bot");
    return result;
}

DECLARE_PARSER_TO_JSON(ManagedBotCreated) {
    JsonWrapper json;
    if (object) {
        json.put("bot", object->bot);
    }
    return json;
}

} // namespace TgBot
