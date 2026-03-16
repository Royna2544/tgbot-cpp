#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ResponseParameters) {
    auto result = std::make_shared<ResponseParameters>();
    parse(data, "migrate_to_chat_id", &result->migrateToChatId);
    parse(data, "retry_after", &result->retryAfter);
    return result;
}

DECLARE_PARSER_TO_JSON(ResponseParameters) {
    JsonWrapper json;
    if (object) {
        json.put("migrate_to_chat_id", object->migrateToChatId);
        json.put("retry_after", object->retryAfter);
    }
    return json;
}

} // namespace TgBot
