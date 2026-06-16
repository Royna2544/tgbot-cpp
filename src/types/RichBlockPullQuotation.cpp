#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockPullQuotation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockPullQuotation) {
    auto result = std::make_shared<RichBlockPullQuotation>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    result->credit = parse<RichText>(data, "credit");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockPullQuotation) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("credit", object->credit);
    }
    return json;
}

} // namespace TgBot
