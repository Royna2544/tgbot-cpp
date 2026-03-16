#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MaskPosition) {
    auto result = std::make_shared<MaskPosition>();
    parse(data, "point", &result->point);
    parse(data, "x_shift", &result->xShift);
    parse(data, "y_shift", &result->yShift);
    parse(data, "scale", &result->scale);
    return result;
}

DECLARE_PARSER_TO_JSON(MaskPosition) {
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
