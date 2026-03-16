#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(OwnedGiftRegular) {
    auto result = std::make_shared<OwnedGiftRegular>();
    parse(data, "type", &result->type);
    result->gift = parseRequired<Gift>(data, "gift");
    parse(data, "owned_gift_id", &result->ownedGiftId);
    result->senderUser = parse<User>(data, "sender_user");
    parse(data, "send_date", &result->sendDate);
    parse(data, "text", &result->text);
    result->entities = parseArray<MessageEntity>(data, "entities");
    parse(data, "is_private", &result->isPrivate);
    parse(data, "is_saved", &result->isSaved);
    parse(data, "can_be_upgraded", &result->canBeUpgraded);
    parse(data, "was_refunded", &result->wasRefunded);
    parse(data, "convert_star_count", &result->convertStarCount);
    parse(data, "prepaid_upgrade_star_count", &result->prepaidUpgradeStarCount);
    parse(data, "is_upgrade_separate", &result->isUpgradeSeparate);
    parse(data, "unique_gift_number", &result->uniqueGiftNumber);
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGiftRegular) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("gift", object->gift);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("sender_user", object->senderUser);
        json.put("send_date", object->sendDate);
        json.put("text", object->text);
        json.put("entities", object->entities);
        json.put("is_private", object->isPrivate);
        json.put("is_saved", object->isSaved);
        json.put("can_be_upgraded", object->canBeUpgraded);
        json.put("was_refunded", object->wasRefunded);
        json.put("convert_star_count", object->convertStarCount);
        json.put("prepaid_upgrade_star_count", object->prepaidUpgradeStarCount);
        json.put("is_upgrade_separate", object->isUpgradeSeparate);
        json.put("unique_gift_number", object->uniqueGiftNumber);
    }
    return json;
}

} // namespace TgBot
