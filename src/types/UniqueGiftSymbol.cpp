#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UniqueGiftSymbol.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UniqueGiftSymbol> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UniqueGiftSymbol>();
    parse(data, "name", &result->name);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UniqueGiftSymbol> &object) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("sticker", object->sticker);
        json.put("rarity_per_mille", object->rarityPerMille);
    }
    return json;
}

} // namespace TgBot
