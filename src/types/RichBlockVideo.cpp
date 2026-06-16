#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockVideo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockVideo>();
    parse(data, "type", &result->type);
    result->video = parseRequired<Video>(data, "video");
    parse(data, "has_spoiler", &result->hasSpoiler);
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockVideo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("video", object->video);
        json.put("has_spoiler", object->hasSpoiler);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
