#include <tgbot/TgTypeParser.h>
#include <tgbot/types/OwnedGifts.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<OwnedGifts> parse(const nlohmann::json &data) {
    auto result = std::make_shared<OwnedGifts>();
    parse(data, "total_count", &result->totalCount);
    result->gifts = parseRequiredArray<OwnedGift>(data, "gifts");
    parse(data, "next_offset", &result->nextOffset);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<OwnedGifts> &object) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("gifts", object->gifts);
        json.put("next_offset", object->nextOffset);
    }
    return json;
}

} // namespace TgBot
