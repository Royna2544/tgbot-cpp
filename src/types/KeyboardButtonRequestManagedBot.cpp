#include <tgbot/TgTypeParser.h>
#include <tgbot/types/KeyboardButtonRequestManagedBot.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<KeyboardButtonRequestManagedBot> parse(const nlohmann::json &data) {
    auto result = std::make_shared<KeyboardButtonRequestManagedBot>();
    parse(data, "request_id", &result->requestId);
    parse(data, "suggested_name", &result->suggestedName);
    parse(data, "suggested_username", &result->suggestedUsername);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<KeyboardButtonRequestManagedBot> &object) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("suggested_name", object->suggestedName);
        json.put("suggested_username", object->suggestedUsername);
    }
    return json;
}

} // namespace TgBot
