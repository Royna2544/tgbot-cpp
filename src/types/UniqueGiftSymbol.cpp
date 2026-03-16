#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UniqueGiftSymbol) {
    auto result = std::make_shared<UniqueGiftSymbol>();
    parse(data, "name", &result->name);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftSymbol) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("sticker", object->sticker);
        json.put("rarity_per_mille", object->rarityPerMille);
    }
    return json;
}

} // namespace TgBot
