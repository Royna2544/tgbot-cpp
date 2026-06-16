#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaDocument.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputMediaDocument> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputMediaDocument>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "thumbnail", &result->thumbnail);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "disable_content_type_detection", &result->disableContentTypeDetection);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputMediaDocument> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("thumbnail", object->thumbnail);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("disable_content_type_detection", object->disableContentTypeDetection);
    }
    return json;
}

} // namespace TgBot
