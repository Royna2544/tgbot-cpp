#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MaskPosition.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MaskPosition> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MaskPosition>();
    parse(data, "point", &result->point);
    parse(data, "x_shift", &result->xShift);
    parse(data, "y_shift", &result->yShift);
    parse(data, "scale", &result->scale);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MaskPosition> &object) {
    JsonWrapper json;
    if (object) {
        json.put("point", object->point);
        json.put("x_shift", object->xShift);
        json.put("y_shift", object->yShift);
        json.put("scale", object->scale);
    }
    return json;
}

} // namespace TgBot
