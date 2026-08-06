#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockVideo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockVideo>();
    parse(data, "type", &result->type);
    result->video = parseRequired<InputMediaVideo>(data, "video");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockVideo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("video", object->video);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
