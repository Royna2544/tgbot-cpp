#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StoryArea.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StoryArea> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StoryArea>();
    result->position = parseRequired<StoryAreaPosition>(data, "position");
    result->type = parseRequired<StoryAreaType>(data, "type");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StoryArea> &object) {
    JsonWrapper json;
    if (object) {
        json.put("position", object->position);
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
