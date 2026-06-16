#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockTable.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockTable) {
    auto result = std::make_shared<RichBlockTable>();
    parse(data, "type", &result->type);
    result->cells = parseMatrix<RichBlockTableCell>(data, "cells");
    parse(data, "is_bordered", &result->isBordered);
    parse(data, "is_striped", &result->isStriped);
    result->caption = parse<RichText>(data, "caption");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockTable) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("cells", object->cells);
        json.put("is_bordered", object->isBordered);
        json.put("is_striped", object->isStriped);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
