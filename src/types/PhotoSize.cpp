#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PhotoSize.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PhotoSize> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PhotoSize>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "file_size", &result->fileSize);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PhotoSize> &object) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("file_size", object->fileSize);
    }
    return json;
}

} // namespace TgBot
