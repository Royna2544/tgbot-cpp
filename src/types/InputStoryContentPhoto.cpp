#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputStoryContentPhoto) {
    auto result = std::make_shared<InputStoryContentPhoto>();
    parse(data, "type", &result->type);
    parse(data, "photo", &result->photo);
    return result;
}

DECLARE_PARSER_TO_JSON(InputStoryContentPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
