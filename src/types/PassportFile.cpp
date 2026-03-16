#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PassportFile) {
    auto result = std::make_shared<PassportFile>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "file_size", &result->fileSize);
    parse(data, "file_date", &result->fileDate);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportFile) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("file_size", object->fileSize);
        json.put("file_date", object->fileDate);
    }
    return json;
}

} // namespace TgBot
