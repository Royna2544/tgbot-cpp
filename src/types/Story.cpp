#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Story) {
    auto result = std::make_shared<Story>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "id", &result->id);
    return result;
}

DECLARE_PARSER_TO_JSON(Story) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("id", object->id);
    }
    return json;
}

} // namespace TgBot
