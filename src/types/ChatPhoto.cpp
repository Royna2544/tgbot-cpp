#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatPhoto) {
    auto result = std::make_shared<ChatPhoto>();
    parse(data, "small_file_id", &result->smallFileId);
    parse(data, "small_file_unique_id", &result->smallFileUniqueId);
    parse(data, "big_file_id", &result->bigFileId);
    parse(data, "big_file_unique_id", &result->bigFileUniqueId);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatPhoto) {
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
