#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundTypeFill.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundTypeFill> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BackgroundTypeFill>();
    parse(data, "type", &result->type);
    result->fill = parseRequired<BackgroundFill>(data, "fill");
    parse(data, "dark_theme_dimming", &result->darkThemeDimming);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BackgroundTypeFill> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("fill", object->fill);
        json.put("dark_theme_dimming", object->darkThemeDimming);
    }
    return json;
}

} // namespace TgBot
