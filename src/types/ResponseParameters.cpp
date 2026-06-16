#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ResponseParameters.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ResponseParameters> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ResponseParameters>();
    parse(data, "migrate_to_chat_id", &result->migrateToChatId);
    parse(data, "retry_after", &result->retryAfter);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ResponseParameters> &object) {
    JsonWrapper json;
    if (object) {
        json.put("migrate_to_chat_id", object->migrateToChatId);
        json.put("retry_after", object->retryAfter);
    }
    return json;
}

} // namespace TgBot
