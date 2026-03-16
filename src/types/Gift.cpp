#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Gift) {
    auto result = std::make_shared<Gift>();
    parse(data, "id", &result->id);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "star_count", &result->starCount);
    parse(data, "upgrade_star_count", &result->upgradeStarCount);
    parse(data, "is_premium", &result->isPremium);
    parse(data, "has_colors", &result->hasColors);
    parse(data, "total_count", &result->totalCount);
    parse(data, "remaining_count", &result->remainingCount);
    parse(data, "personal_total_count", &result->personalTotalCount);
    parse(data, "personal_remaining_count", &result->personalRemainingCount);
    result->background = parse<GiftBackground>(data, "background");
    parse(data, "unique_gift_variant_count", &result->uniqueGiftVariantCount);
    result->publisherChat = parse<Chat>(data, "publisher_chat");
    return result;
}

DECLARE_PARSER_TO_JSON(Gift) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("sticker", object->sticker);
        json.put("star_count", object->starCount);
        json.put("upgrade_star_count", object->upgradeStarCount);
        json.put("is_premium", object->isPremium);
        json.put("has_colors", object->hasColors);
        json.put("total_count", object->totalCount);
        json.put("remaining_count", object->remainingCount);
        json.put("personal_total_count", object->personalTotalCount);
        json.put("personal_remaining_count", object->personalRemainingCount);
        json.put("background", object->background);
        json.put("unique_gift_variant_count", object->uniqueGiftVariantCount);
        json.put("publisher_chat", object->publisherChat);
    }
    return json;
}

} // namespace TgBot
