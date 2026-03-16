#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputProfilePhotoAnimated) {
    auto result = std::make_shared<InputProfilePhotoAnimated>();
    parse(data, "type", &result->type);
    parse(data, "animation", &result->animation);
    parse(data, "main_frame_timestamp", &result->mainFrameTimestamp);
    return result;
}

DECLARE_PARSER_TO_JSON(InputProfilePhotoAnimated) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("animation", object->animation);
        json.put("main_frame_timestamp", object->mainFrameTimestamp);
    }
    return json;
}

} // namespace TgBot
