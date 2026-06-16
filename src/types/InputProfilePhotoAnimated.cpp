#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputProfilePhotoAnimated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputProfilePhotoAnimated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputProfilePhotoAnimated>();
    parse(data, "type", &result->type);
    parse(data, "animation", &result->animation);
    parse(data, "main_frame_timestamp", &result->mainFrameTimestamp);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputProfilePhotoAnimated> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("animation", object->animation);
        json.put("main_frame_timestamp", object->mainFrameTimestamp);
    }
    return json;
}

} // namespace TgBot
