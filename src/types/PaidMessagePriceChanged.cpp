#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMessagePriceChanged) {
    auto result = std::make_shared<PaidMessagePriceChanged>();
    parse(data, "paid_message_star_count", &result->paidMessageStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMessagePriceChanged) {
    JsonWrapper json;
    if (object) {
        json.put("paid_message_star_count", object->paidMessageStarCount);
    }
    return json;
}

} // namespace TgBot
