#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockList.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockList) {
    auto result = std::make_shared<RichBlockList>();
    parse(data, "type", &result->type);
    result->items = parseRequiredArray<RichBlockListItem>(data, "items");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockList) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("items", object->items);
    }
    return json;
}

} // namespace TgBot
