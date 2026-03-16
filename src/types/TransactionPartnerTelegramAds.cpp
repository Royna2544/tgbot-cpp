#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(TransactionPartnerTelegramAds) {
    auto result = std::make_shared<TransactionPartnerTelegramAds>();
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerTelegramAds) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
