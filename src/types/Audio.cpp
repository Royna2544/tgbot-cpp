#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Audio.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Audio> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Audio>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "duration", &result->duration);
    parse(data, "performer", &result->performer);
    parse(data, "title", &result->title);
    parse(data, "file_name", &result->fileName);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Audio> &object) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("duration", object->duration);
        json.put("performer", object->performer);
        json.put("title", object->title);
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
        json.put("thumbnail", object->thumbnail);
    }
    return json;
}

} // namespace TgBot
