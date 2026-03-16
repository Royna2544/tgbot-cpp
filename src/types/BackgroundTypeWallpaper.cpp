#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundTypeWallpaper) {
    auto result = std::make_shared<BackgroundTypeWallpaper>();
    parse(data, "type", &result->type);
    result->document = parseRequired<Document>(data, "document");
    parse(data, "dark_theme_dimming", &result->darkThemeDimming);
    parse(data, "is_blurred", &result->isBlurred);
    parse(data, "is_moving", &result->isMoving);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypeWallpaper) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("document", object->document);
        json.put("dark_theme_dimming", object->darkThemeDimming);
        json.put("is_blurred", object->isBlurred);
        json.put("is_moving", object->isMoving);
    }
    return json;
}

} // namespace TgBot
