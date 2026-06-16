#include <tgbot/TgTypeParser.h>
#include <tgbot/types/File.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<File> parse(const nlohmann::json &data) {
    auto result = std::make_shared<File>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "file_size", &result->fileSize);
    parse(data, "file_path", &result->filePath);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<File> &object) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("file_size", object->fileSize);
        json.put("file_path", object->filePath);
    }
    return json;
}

} // namespace TgBot
