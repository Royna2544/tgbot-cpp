#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockVideo) {
    auto result = std::make_shared<RichBlockVideo>();
    parse(data, "type", &result->type);
    result->video = parseRequired<Video>(data, "video");
    parse(data, "has_spoiler", &result->hasSpoiler);
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockVideo) {
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
