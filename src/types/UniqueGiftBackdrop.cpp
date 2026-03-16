#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UniqueGiftBackdrop) {
    auto result = std::make_shared<UniqueGiftBackdrop>();
    parse(data, "name", &result->name);
    result->colors = parseRequired<UniqueGiftBackdropColors>(data, "colors");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftBackdrop) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("colors", object->colors);
        json.put("rarity_per_mille", object->rarityPerMille);
    }
    return json;
}

} // namespace TgBot
