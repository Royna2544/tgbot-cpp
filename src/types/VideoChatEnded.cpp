#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(VideoChatEnded) {
    auto result = std::make_shared<VideoChatEnded>();
    parse(data, "duration", &result->duration);
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatEnded) {
    JsonWrapper json;
    if (object) {
        json.put("duration", object->duration);
    }
    return json;
}

} // namespace TgBot
