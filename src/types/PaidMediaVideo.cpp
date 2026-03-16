#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMediaVideo) {
    auto result = std::make_shared<PaidMediaVideo>();
    parse(data, "type", &result->type);
    result->video = parseRequired<Video>(data, "video");
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaVideo) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("video", object->video);
    }
    return json;
}

} // namespace TgBot
