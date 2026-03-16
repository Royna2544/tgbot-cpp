#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(VideoNote) {
    auto result = std::make_shared<VideoNote>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "length", &result->length);
    parse(data, "duration", &result->duration);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "file_size", &result->fileSize);
    return result;
}

DECLARE_PARSER_TO_JSON(VideoNote) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("length", object->length);
        json.put("duration", object->duration);
        json.put("thumbnail", object->thumbnail);
        json.put("file_size", object->fileSize);
    }
    return json;
}

} // namespace TgBot
