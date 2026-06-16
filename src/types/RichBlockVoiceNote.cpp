#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockVoiceNote.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockVoiceNote) {
    auto result = std::make_shared<RichBlockVoiceNote>();
    parse(data, "type", &result->type);
    result->voiceNote = parseRequired<Voice>(data, "voice_note");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockVoiceNote) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("voice_note", object->voiceNote);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
