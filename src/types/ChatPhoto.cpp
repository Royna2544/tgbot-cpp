#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatPhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatPhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatPhoto>();
    parse(data, "small_file_id", &result->smallFileId);
    parse(data, "small_file_unique_id", &result->smallFileUniqueId);
    parse(data, "big_file_id", &result->bigFileId);
    parse(data, "big_file_unique_id", &result->bigFileUniqueId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatPhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("small_file_id", object->smallFileId);
        json.put("small_file_unique_id", object->smallFileUniqueId);
        json.put("big_file_id", object->bigFileId);
        json.put("big_file_unique_id", object->bigFileUniqueId);
    }
    return json;
}

} // namespace TgBot
