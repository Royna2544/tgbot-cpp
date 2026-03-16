#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Document) {
    auto result = std::make_shared<Document>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "file_name", &result->fileName);
    parse(data, "mime_type", &result->mimeType);
    parse(data, "file_size", &result->fileSize);
    return result;
}

DECLARE_PARSER_TO_JSON(Document) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("thumbnail", object->thumbnail);
        json.put("file_name", object->fileName);
        json.put("mime_type", object->mimeType);
        json.put("file_size", object->fileSize);
    }
    return json;
}

} // namespace TgBot
