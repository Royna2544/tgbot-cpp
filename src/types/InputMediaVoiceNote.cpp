#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaVoiceNote.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputMediaVoiceNote> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputMediaVoiceNote>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "duration", &result->duration);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputMediaVoiceNote> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("duration", object->duration);
    }
    return json;
}

} // namespace TgBot
