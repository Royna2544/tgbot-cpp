#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMediaPreview) {
    auto result = std::make_shared<PaidMediaPreview>();
    parse(data, "type", &result->type);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaPreview) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
    }
    return json;
}

} // namespace TgBot
