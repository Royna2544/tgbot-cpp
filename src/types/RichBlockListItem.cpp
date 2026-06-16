#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockListItem.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockListItem) {
    auto result = std::make_shared<RichBlockListItem>();
    parse(data, "label", &result->label);
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    parse(data, "has_checkbox", &result->hasCheckbox);
    parse(data, "is_checked", &result->isChecked);
    parse(data, "value", &result->value);
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockListItem) {
    JsonWrapper json;
    if (object) {
        json.put("label", object->label);
        json.put("blocks", object->blocks);
        json.put("has_checkbox", object->hasCheckbox);
        json.put("is_checked", object->isChecked);
        json.put("value", object->value);
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
