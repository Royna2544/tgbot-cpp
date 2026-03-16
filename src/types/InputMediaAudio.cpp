#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputMediaAudio) {
    auto result = std::make_shared<InputMediaAudio>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "duration", &result->duration);
    parse(data, "performer", &result->performer);
    parse(data, "title", &result->title);
    return result;
}

DECLARE_PARSER_TO_JSON(InputMediaAudio) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("thumbnail", object->thumbnail);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("duration", object->duration);
        json.put("performer", object->performer);
        json.put("title", object->title);
    }
    return json;
}

} // namespace TgBot
