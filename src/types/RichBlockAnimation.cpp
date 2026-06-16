#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockAnimation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockAnimation) {
    auto result = std::make_shared<RichBlockAnimation>();
    parse(data, "type", &result->type);
    result->animation = parseRequired<Animation>(data, "animation");
    parse(data, "has_spoiler", &result->hasSpoiler);
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockAnimation) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("animation", object->animation);
        json.put("has_spoiler", object->hasSpoiler);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
