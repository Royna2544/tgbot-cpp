#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundTypeFill) {
    auto result = std::make_shared<BackgroundTypeFill>();
    parse(data, "type", &result->type);
    result->fill = parseRequired<BackgroundFill>(data, "fill");
    parse(data, "dark_theme_dimming", &result->darkThemeDimming);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypeFill) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("fill", object->fill);
        json.put("dark_theme_dimming", object->darkThemeDimming);
    }
    return json;
}

} // namespace TgBot
