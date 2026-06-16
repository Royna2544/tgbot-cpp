#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputPaidMediaVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputPaidMediaVideo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputPaidMediaVideo>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "cover", &result->cover);
    parse(data, "start_timestamp", &result->startTimestamp);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    parse(data, "supports_streaming", &result->supportsStreaming);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputPaidMediaVideo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("thumbnail", object->thumbnail);
        json.put("cover", object->cover);
        json.put("start_timestamp", object->startTimestamp);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
        json.put("supports_streaming", object->supportsStreaming);
    }
    return json;
}

} // namespace TgBot
