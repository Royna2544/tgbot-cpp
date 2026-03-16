#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(VideoQuality) {
    auto result = std::make_shared<VideoQuality>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "codec", &result->codec);
    parse(data, "file_size", &result->fileSize);
    return result;
}

DECLARE_PARSER_TO_JSON(VideoQuality) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("codec", object->codec);
        json.put("file_size", object->fileSize);
    }
    return json;
}

} // namespace TgBot
