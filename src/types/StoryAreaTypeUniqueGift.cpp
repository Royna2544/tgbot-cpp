#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StoryAreaTypeUniqueGift) {
    auto result = std::make_shared<StoryAreaTypeUniqueGift>();
    parse(data, "type", &result->type);
    parse(data, "name", &result->name);
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeUniqueGift) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
