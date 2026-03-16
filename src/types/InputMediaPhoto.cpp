#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputMediaPhoto) {
    auto result = std::make_shared<InputMediaPhoto>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
    parse(data, "has_spoiler", &result->hasSpoiler);
    return result;
}

DECLARE_PARSER_TO_JSON(InputMediaPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("show_caption_above_media", object->showCaptionAboveMedia);
        json.put("has_spoiler", object->hasSpoiler);
    }
    return json;
}

} // namespace TgBot
