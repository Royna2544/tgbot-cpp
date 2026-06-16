#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundFillGradient.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundFillGradient> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BackgroundFillGradient>();
    parse(data, "type", &result->type);
    parse(data, "top_color", &result->topColor);
    parse(data, "bottom_color", &result->bottomColor);
    parse(data, "rotation_angle", &result->rotationAngle);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BackgroundFillGradient> &object) {
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
