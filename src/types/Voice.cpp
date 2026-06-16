#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Voice.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Voice> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Voice>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "duration", &result->duration);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Voice> &object) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("duration", object->duration);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }
    return json;
}

} // namespace TgBot
