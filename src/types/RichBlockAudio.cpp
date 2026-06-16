#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockAudio.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockAudio) {
    auto result = std::make_shared<RichBlockAudio>();
    parse(data, "type", &result->type);
    result->audio = parseRequired<Audio>(data, "audio");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockAudio) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("audio", object->audio);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
