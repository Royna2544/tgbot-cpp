#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PreparedInlineMessage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PreparedInlineMessage> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PreparedInlineMessage>();
    parse(data, "id", &result->id);
    parse(data, "expiration_date", &result->expirationDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PreparedInlineMessage> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("expiration_date", object->expirationDate);
    }
    return json;
}

} // namespace TgBot
