#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMediaInfo) {
    auto result = std::make_shared<PaidMediaInfo>();
    parse(data, "star_count", &result->starCount);
    result->paidMedia = parseRequiredArray<PaidMedia>(data, "paid_media");
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaInfo) {
    JsonWrapper json;
    if (object) {
        json.put("star_count", object->starCount);
        json.put("paid_media", object->paidMedia);
    }
    return json;
}

} // namespace TgBot
