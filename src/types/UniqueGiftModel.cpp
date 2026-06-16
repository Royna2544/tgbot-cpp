#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UniqueGiftModel.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UniqueGiftModel> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UniqueGiftModel>();
    parse(data, "name", &result->name);
    result->sticker = parseRequired<Sticker>(data, "sticker");
    parse(data, "rarity_per_mille", &result->rarityPerMille);
    parse(data, "rarity", &result->rarity);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UniqueGiftModel> &object) {
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
