#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StoryAreaTypeWeather.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StoryAreaTypeWeather> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StoryAreaTypeWeather>();
    parse(data, "type", &result->type);
    parse(data, "temperature", &result->temperature);
    parse(data, "emoji", &result->emoji);
    parse(data, "background_color", &result->backgroundColor);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StoryAreaTypeWeather> &object) {
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
