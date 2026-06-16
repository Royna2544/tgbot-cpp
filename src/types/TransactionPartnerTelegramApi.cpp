#include <tgbot/TgTypeParser.h>
#include <tgbot/types/TransactionPartnerTelegramApi.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<TransactionPartnerTelegramApi> parse(const nlohmann::json &data) {
    auto result = std::make_shared<TransactionPartnerTelegramApi>();
    parse(data, "type", &result->type);
    parse(data, "request_count", &result->requestCount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<TransactionPartnerTelegramApi> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("request_count", object->requestCount);
    }
    return json;
}

} // namespace TgBot
