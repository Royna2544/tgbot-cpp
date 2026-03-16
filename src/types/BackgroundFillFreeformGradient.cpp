#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundFillFreeformGradient) {
    auto result = std::make_shared<BackgroundFillFreeformGradient>();
    parse(data, "type", &result->type);
    result->colors = parsePrimitiveRequiredArray<std::int64_t>(data, "colors");
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundFillFreeformGradient) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("colors", object->colors);
    }
    return json;
}

} // namespace TgBot
