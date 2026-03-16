#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(AcceptedGiftTypes) {
    auto result = std::make_shared<AcceptedGiftTypes>();
    parse(data, "unlimited_gifts", &result->unlimitedGifts);
    parse(data, "limited_gifts", &result->limitedGifts);
    parse(data, "unique_gifts", &result->uniqueGifts);
    parse(data, "premium_subscription", &result->premiumSubscription);
    parse(data, "gifts_from_channels", &result->giftsFromChannels);
    return result;
}

DECLARE_PARSER_TO_JSON(AcceptedGiftTypes) {
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
