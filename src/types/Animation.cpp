#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Animation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Animation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Animation>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "file_name", &result->fileName);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Animation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
        json.put("thumbnail", object->thumbnail);
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }
    return json;
}

} // namespace TgBot
