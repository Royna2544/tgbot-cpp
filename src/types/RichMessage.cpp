#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichMessage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichMessage) {
    auto result = std::make_shared<RichMessage>();
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    parse(data, "is_rtl", &result->isRtl);
    return result;
}

DECLARE_PARSER_TO_JSON(RichMessage) {
    JsonWrapper json;
    if (object) {
        json.put("blocks", object->blocks);
        json.put("is_rtl", object->isRtl);
    }
    return json;
}

} // namespace TgBot
