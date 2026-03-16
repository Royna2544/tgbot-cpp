#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputStoryContentVideo) {
    auto result = std::make_shared<InputStoryContentVideo>();
    parse(data, "type", &result->type);
    parse(data, "video", &result->video);
    parse(data, "duration", &result->duration);
    parse(data, "cover_frame_timestamp", &result->coverFrameTimestamp);
    parse(data, "is_animation", &result->isAnimation);
    return result;
}

DECLARE_PARSER_TO_JSON(InputStoryContentVideo) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("video", object->video);
        json.put("duration", object->duration);
        json.put("cover_frame_timestamp", object->coverFrameTimestamp);
        json.put("is_animation", object->isAnimation);
    }
    return json;
}

} // namespace TgBot
