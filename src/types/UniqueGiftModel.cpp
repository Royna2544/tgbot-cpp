#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UniqueGiftModel) {
    auto result = std::make_shared<UniqueGiftModel>();
    parse(data, "name", &result->name);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    parse(data, "rarity", &result->rarity);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftModel) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("sticker", object->sticker);
        json.put("rarity_per_mille", object->rarityPerMille);
        json.put("rarity", object->rarity);
    }
    return json;
}

} // namespace TgBot
