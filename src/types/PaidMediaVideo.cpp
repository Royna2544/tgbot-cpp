#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMediaVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMediaVideo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PaidMediaVideo>();
    parse(data, "type", &result->type);
    result->video = parseRequired<Video>(data, "video");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMediaVideo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("video", object->video);
    }
    return json;
}

} // namespace TgBot
