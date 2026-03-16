#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(TransactionPartnerTelegramApi) {
    auto result = std::make_shared<TransactionPartnerTelegramApi>();
    parse(data, "type", &result->type);
    parse(data, "request_count", &result->requestCount);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerTelegramApi) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("request_count", object->requestCount);
    }
    return json;
}

} // namespace TgBot
