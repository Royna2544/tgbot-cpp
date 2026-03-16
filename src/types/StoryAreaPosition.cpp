#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StoryAreaPosition) {
    auto result = std::make_shared<StoryAreaPosition>();
    parse(data, "x_percentage", &result->xPercentage);
    parse(data, "y_percentage", &result->yPercentage);
    parse(data, "width_percentage", &result->widthPercentage);
    parse(data, "height_percentage", &result->heightPercentage);
    parse(data, "rotation_angle", &result->rotationAngle);
    parse(data, "corner_radius_percentage", &result->cornerRadiusPercentage);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaPosition) {
    JsonWrapper json;
    if (object) {
        json.put("x_percentage", object->xPercentage);
        json.put("y_percentage", object->yPercentage);
        json.put("width_percentage", object->widthPercentage);
        json.put("height_percentage", object->heightPercentage);
        json.put("rotation_angle", object->rotationAngle);
        json.put("corner_radius_percentage", object->cornerRadiusPercentage);
    }
    return json;
}

} // namespace TgBot
