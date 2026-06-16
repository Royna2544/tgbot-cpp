#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextBotCommand.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextBotCommand> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextBotCommand>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "bot_command", &result->botCommand);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextBotCommand> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("bot_command", object->botCommand);
    }
    return json;
}

} // namespace TgBot
