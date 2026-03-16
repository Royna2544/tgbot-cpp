#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Voice) {
    auto result = std::make_shared<Voice>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "duration", &result->duration);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
    return result;
}

DECLARE_PARSER_TO_JSON(Voice) {
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
