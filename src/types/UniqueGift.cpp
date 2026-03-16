#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UniqueGift) {
    auto result = std::make_shared<UniqueGift>();
    parse(data, "gift_id", &result->giftId);
    parse(data, "base_name", &result->baseName);
    parse(data, "name", &result->name);
    parse(data, "number", &result->number);
    result->model = parseRequired<UniqueGiftModel>(data, "model");
    result->symbol = parseRequired<UniqueGiftSymbol>(data, "symbol");
    result->backdrop = parseRequired<UniqueGiftBackdrop>(data, "backdrop");
    parse(data, "is_premium", &result->isPremium);
    parse(data, "is_burned", &result->isBurned);
    parse(data, "is_from_blockchain", &result->isFromBlockchain);
    result->colors = parse<UniqueGiftColors>(data, "colors");
    result->publisherChat = parse<Chat>(data, "publisher_chat");
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGift) {
    JsonWrapper json;
    if (object) {
        json.put("gift_id", object->giftId);
        json.put("base_name", object->baseName);
        json.put("name", object->name);
        json.put("number", object->number);
        json.put("model", object->model);
        json.put("symbol", object->symbol);
        json.put("backdrop", object->backdrop);
        json.put("is_premium", object->isPremium);
        json.put("is_burned", object->isBurned);
        json.put("is_from_blockchain", object->isFromBlockchain);
        json.put("colors", object->colors);
        json.put("publisher_chat", object->publisherChat);
    }
    return json;
}

} // namespace TgBot
