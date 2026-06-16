#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaLivePhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputMediaLivePhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputMediaLivePhoto>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "photo", &result->photo);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "show_caption_above_media", &result->showCaptionAboveMedia);
    parse(data, "has_spoiler", &result->hasSpoiler);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputMediaLivePhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("photo", object->photo);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("show_caption_above_media", object->showCaptionAboveMedia);
        json.put("has_spoiler", object->hasSpoiler);
    }
    return json;
}

} // namespace TgBot
