#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockDetails.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockDetails) {
    auto result = std::make_shared<RichBlockDetails>();
    parse(data, "type", &result->type);
    result->summary = parseRequired<RichText>(data, "summary");
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    parse(data, "is_open", &result->isOpen);
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockDetails) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("summary", object->summary);
        json.put("blocks", object->blocks);
        json.put("is_open", object->isOpen);
    }
    return json;
}

} // namespace TgBot
