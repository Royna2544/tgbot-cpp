#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundFillSolid) {
    auto result = std::make_shared<BackgroundFillSolid>();
    parse(data, "type", &result->type);
    parse(data, "color", &result->color);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundFillSolid) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("color", object->color);
    }
    return json;
}

} // namespace TgBot
