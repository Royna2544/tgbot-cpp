#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputStoryContentVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputStoryContentVideo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputStoryContentVideo>();
    parse(data, "type", &result->type);
    parse(data, "video", &result->video);
    parse(data, "duration", &result->duration);
    parse(data, "cover_frame_timestamp", &result->coverFrameTimestamp);
    parse(data, "is_animation", &result->isAnimation);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputStoryContentVideo> &object) {
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
