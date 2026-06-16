#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockAnimation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockAnimation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockAnimation>();
    parse(data, "type", &result->type);
    result->animation = parseRequired<Animation>(data, "animation");
    parse(data, "has_spoiler", &result->hasSpoiler);
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockAnimation> &object) {
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
