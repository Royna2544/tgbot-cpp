#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundFillGradient) {
    auto result = std::make_shared<BackgroundFillGradient>();
    parse(data, "type", &result->type);
    parse(data, "top_color", &result->topColor);
    parse(data, "bottom_color", &result->bottomColor);
    parse(data, "rotation_angle", &result->rotationAngle);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundFillGradient) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("top_color", object->topColor);
        json.put("bottom_color", object->bottomColor);
        json.put("rotation_angle", object->rotationAngle);
    }
    return json;
}

} // namespace TgBot
