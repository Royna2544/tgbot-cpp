#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundTypePattern) {
    auto result = std::make_shared<BackgroundTypePattern>();
    parse(data, "type", &result->type);
    result->document = parseRequired<Document>(data, "document");
    result->fill = parseRequired<BackgroundFill>(data, "fill");
    parse(data, "intensity", &result->intensity);
    parse(data, "is_inverted", &result->isInverted);
    parse(data, "is_moving", &result->isMoving);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypePattern) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("document", object->document);
        json.put("fill", object->fill);
        json.put("intensity", object->intensity);
        json.put("is_inverted", object->isInverted);
        json.put("is_moving", object->isMoving);
    }
    return json;
}

} // namespace TgBot
