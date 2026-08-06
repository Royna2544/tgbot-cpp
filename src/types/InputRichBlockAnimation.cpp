#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockAnimation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockAnimation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockAnimation>();
    parse(data, "type", &result->type);
    result->animation = parseRequired<InputMediaAnimation>(data, "animation");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockAnimation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("animation", object->animation);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
