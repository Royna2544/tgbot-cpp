#include <tgbot/TgTypeParser.h>
#include <tgbot/types/AcceptedGiftTypes.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<AcceptedGiftTypes> parse(const nlohmann::json &data) {
    auto result = std::make_shared<AcceptedGiftTypes>();
    parse(data, "unlimited_gifts", &result->unlimitedGifts);
    parse(data, "limited_gifts", &result->limitedGifts);
    parse(data, "unique_gifts", &result->uniqueGifts);
    parse(data, "premium_subscription", &result->premiumSubscription);
    parse(data, "gifts_from_channels", &result->giftsFromChannels);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<AcceptedGiftTypes> &object) {
    JsonWrapper json;
    if (object) {
        json.put("unlimited_gifts", object->unlimitedGifts);
        json.put("limited_gifts", object->limitedGifts);
        json.put("unique_gifts", object->uniqueGifts);
        json.put("premium_subscription", object->premiumSubscription);
        json.put("gifts_from_channels", object->giftsFromChannels);
    }
    return json;
}

} // namespace TgBot
