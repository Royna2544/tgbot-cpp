#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StoryAreaType) {
    std::string type;
    StoryAreaType::Ptr result;
    parse(data, "type", &type);
    if (type == "location") {
        result = parse<StoryAreaTypeLocation>(data);
    } else if (type == "suggested_reaction") {
        result = parse<StoryAreaTypeSuggestedReaction>(data);
    } else if (type == "link") {
        result = parse<StoryAreaTypeLink>(data);
    } else if (type == "weather") {
        result = parse<StoryAreaTypeWeather>(data);
    } else if (type == "unique_gift") {
        result = parse<StoryAreaTypeUniqueGift>(data);
    } else {
        throw invalidType("StoryAreaType", type);
    }
    return result;
}

DECLARE_PARSER_TO_JSON(StoryAreaType) {
    JsonWrapper json;
    if (object) {
        if (object->type == "location") {
            json += put<StoryAreaTypeLocation>(object);
        } else if (object->type == "suggested_reaction") {
            json += put<StoryAreaTypeSuggestedReaction>(object);
        } else if (object->type == "link") {
            json += put<StoryAreaTypeLink>(object);
        } else if (object->type == "weather") {
            json += put<StoryAreaTypeWeather>(object);
        } else if (object->type == "unique_gift") {
            json += put<StoryAreaTypeUniqueGift>(object);
        } else {
            throw invalidType("StoryAreaType", object->type);
        }
    }
    return json;
}

} // namespace TgBot
