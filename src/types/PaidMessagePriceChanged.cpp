#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMessagePriceChanged.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMessagePriceChanged> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PaidMessagePriceChanged>();
    parse(data, "paid_message_star_count", &result->paidMessageStarCount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMessagePriceChanged> &object) {
    JsonWrapper json;
    if (object) {
        json.put("paid_message_star_count", object->paidMessageStarCount);
    }
    return json;
}

} // namespace TgBot
