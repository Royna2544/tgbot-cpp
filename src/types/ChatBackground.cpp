#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatBackground) {
    auto result = std::make_shared<ChatBackground>();
    result->type = parseRequired<BackgroundType>(data, "type");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBackground) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
