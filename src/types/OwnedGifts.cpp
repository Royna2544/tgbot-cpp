#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(OwnedGifts) {
    auto result = std::make_shared<OwnedGifts>();
    parse(data, "total_count", &result->totalCount);
    result->gifts = parseRequiredArray<OwnedGift>(data, "gifts");
    parse(data, "next_offset", &result->nextOffset);
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGifts) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("gifts", object->gifts);
        json.put("next_offset", object->nextOffset);
    }
    return json;
}

} // namespace TgBot
