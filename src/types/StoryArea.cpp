#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StoryArea) {
    auto result = std::make_shared<StoryArea>();
    result->position = parseRequired<StoryAreaPosition>(data, "position");
    result->type = parseRequired<StoryAreaType>(data, "type");
    return result;
}

DECLARE_PARSER_TO_JSON(StoryArea) {
    JsonWrapper json;
    if (object) {
        json.put("position", object->position);
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
