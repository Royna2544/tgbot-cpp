#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundTypePattern.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundTypePattern> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BackgroundTypePattern>();
    parse(data, "type", &result->type);
    result->document = parseRequired<Document>(data, "document");
    result->fill = parseRequired<BackgroundFill>(data, "fill");
    parse(data, "intensity", &result->intensity);
    parse(data, "is_inverted", &result->isInverted);
    parse(data, "is_moving", &result->isMoving);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BackgroundTypePattern> &object) {
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
