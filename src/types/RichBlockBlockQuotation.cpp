#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockBlockQuotation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockBlockQuotation) {
    auto result = std::make_shared<RichBlockBlockQuotation>();
    parse(data, "type", &result->type);
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    result->credit = parse<RichText>(data, "credit");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockBlockQuotation) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("blocks", object->blocks);
        json.put("credit", object->credit);
    }
    return json;
}

} // namespace TgBot
