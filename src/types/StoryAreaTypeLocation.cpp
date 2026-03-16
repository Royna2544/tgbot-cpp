#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StoryAreaTypeLocation) {
    auto result = std::make_shared<StoryAreaTypeLocation>();
    parse(data, "type", &result->type);
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    result->address = parse<LocationAddress>(data, "address");
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaTypeLocation) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("address", object->address);
    }
    return json;
}

} // namespace TgBot
