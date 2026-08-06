#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockTable.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockTable> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockTable>();
    parse(data, "type", &result->type);
    result->cells = parseMatrix<RichBlockTableCell>(data, "cells");
    parse(data, "is_bordered", &result->isBordered);
    parse(data, "is_striped", &result->isStriped);
    result->caption = parse<RichText>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockTable> &object) {
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
