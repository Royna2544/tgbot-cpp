#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StoryAreaTypeWeather) {
    auto result = std::make_shared<StoryAreaTypeWeather>();
    parse(data, "type", &result->type);
    parse(data, "temperature", &result->temperature);
    parse(data, "emoji", &result->emoji);
    parse(data, "background_color", &result->backgroundColor);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeWeather) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("temperature", object->temperature);
        json.put("emoji", object->emoji);
        json.put("background_color", object->backgroundColor);
    }
    return json;
}

} // namespace TgBot
