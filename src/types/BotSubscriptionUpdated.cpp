#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotSubscriptionUpdated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotSubscriptionUpdated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotSubscriptionUpdated>();
    result->user = parseRequired<User>(data, "user");
    parse(data, "invoice_payload", &result->invoicePayload);
    parse(data, "state", &result->state);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotSubscriptionUpdated> &object) {
    JsonWrapper json;
    if (object) {
        json.put("user", object->user);
        json.put("invoice_payload", object->invoicePayload);
        json.put("state", object->state);
    }
    return json;
}

} // namespace TgBot
