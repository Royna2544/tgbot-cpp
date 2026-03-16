#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GiftInfo) {
    auto result = std::make_shared<GiftInfo>();
    result->gift = parseRequired<Gift>(data, "gift");
    parse(data, "owned_gift_id", &result->ownedGiftId);
    parse(data, "convert_star_count", &result->convertStarCount);
    parse(data, "prepaid_upgrade_star_count", &result->prepaidUpgradeStarCount);
    parse(data, "is_upgrade_separate", &result->isUpgradeSeparate);
    parse(data, "can_be_upgraded", &result->canBeUpgraded);
    parse(data, "text", &result->text);
    result->entities = parseArray<MessageEntity>(data, "entities");
    parse(data, "is_private", &result->isPrivate);
    parse(data, "unique_gift_number", &result->uniqueGiftNumber);
    return result;
}

DECLARE_PARSER_TO_JSON(GiftInfo) {
    JsonWrapper json;
    if (object) {
        json.put("gift", object->gift);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("convert_star_count", object->convertStarCount);
        json.put("prepaid_upgrade_star_count", object->prepaidUpgradeStarCount);
        json.put("is_upgrade_separate", object->isUpgradeSeparate);
        json.put("can_be_upgraded", object->canBeUpgraded);
        json.put("text", object->text);
        json.put("entities", object->entities);
        json.put("is_private", object->isPrivate);
        json.put("unique_gift_number", object->uniqueGiftNumber);
    }
    return json;
}

} // namespace TgBot
