#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UniqueGiftBackdrop.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UniqueGiftBackdrop> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UniqueGiftBackdrop>();
    parse(data, "name", &result->name);
    result->colors = parseRequired<UniqueGiftBackdropColors>(data, "colors");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UniqueGiftBackdrop> &object) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("colors", object->colors);
        json.put("rarity_per_mille", object->rarityPerMille);
    }
    return json;
}

} // namespace TgBot
