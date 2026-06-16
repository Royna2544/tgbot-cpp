#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextBotCommand.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextBotCommand) {
    auto result = std::make_shared<RichTextBotCommand>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "bot_command", &result->botCommand);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextBotCommand) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("bot_command", object->botCommand);
    }
    return json;
}

} // namespace TgBot
