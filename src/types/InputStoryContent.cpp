#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputStoryContent) {
    std::string type;
    InputStoryContent::Ptr result;
    parse(data, "type", &type);
    if (type == "photo") {
        result = parse<InputStoryContentPhoto>(data);
    } else if (type == "video") {
        result = parse<InputStoryContentVideo>(data);
    } else {
        throw invalidType("InputStoryContent", type);
    }
    return result;
}

DECLARE_PARSER_TO_JSON(InputStoryContent) {
    JsonWrapper json;
    if (object) {
        if (object->type == "photo") {
            json += put<InputStoryContentPhoto>(object);
        } else if (object->type == "video") {
            json += put<InputStoryContentVideo>(object);
        } else {
            throw invalidType("InputStoryContent", object->type);
        }
    }
    return json;
}

} // namespace TgBot
