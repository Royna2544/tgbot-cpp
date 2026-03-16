#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(OwnedGiftUnique) {
    auto result = std::make_shared<OwnedGiftUnique>();
    parse(data, "type", &result->type);
    result->gift = parseRequired<UniqueGift>(data, "gift");
    parse(data, "owned_gift_id", &result->ownedGiftId);
    result->senderUser = parse<User>(data, "sender_user");
    parse(data, "send_date", &result->sendDate);
    parse(data, "is_saved", &result->isSaved);
    parse(data, "can_be_transferred", &result->canBeTransferred);
    parse(data, "transfer_star_count", &result->transferStarCount);
    parse(data, "next_transfer_date", &result->nextTransferDate);
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGiftUnique) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("gift", object->gift);
        json.put("owned_gift_id", object->ownedGiftId);
        json.put("sender_user", object->senderUser);
        json.put("send_date", object->sendDate);
        json.put("is_saved", object->isSaved);
        json.put("can_be_transferred", object->canBeTransferred);
        json.put("transfer_star_count", object->transferStarCount);
        json.put("next_transfer_date", object->nextTransferDate);
    }
    return json;
}

} // namespace TgBot
