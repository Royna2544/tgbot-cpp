#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UniqueGiftInfo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UniqueGiftInfo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UniqueGiftInfo>();
    result->gift = parseRequired<UniqueGift>(data, "gift");
    parse(data, "origin", &result->origin);
    parse(data, "last_resale_currency", &result->lastResaleCurrency);
    parse(data, "last_resale_amount", &result->lastResaleAmount);
    parse(data, "owned_gift_id", &result->ownedGiftId);
    parse(data, "transfer_star_count", &result->transferStarCount);
    parse(data, "next_transfer_date", &result->nextTransferDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UniqueGiftInfo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("gift", object->gift);
        json.put("origin", object->origin);
        json.put("last_resale_currency", object->lastResaleCurrency);
        json.put("last_resale_amount", object->lastResaleAmount);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("transfer_star_count", object->transferStarCount);
        json.put("next_transfer_date", object->nextTransferDate);
    }
    return json;
}

} // namespace TgBot
