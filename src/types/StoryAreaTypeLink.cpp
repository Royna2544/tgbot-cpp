#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StoryAreaTypeLink) {
    auto result = std::make_shared<StoryAreaTypeLink>();
    parse(data, "type", &result->type);
    parse(data, "url", &result->url);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeLink) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
