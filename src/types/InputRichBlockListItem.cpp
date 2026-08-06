#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockListItem.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockListItem> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockListItem>();
    result->blocks = parseRequiredArray<InputRichBlock>(data, "blocks");
    parse(data, "has_checkbox", &result->hasCheckbox);
    parse(data, "is_checked", &result->isChecked);
    parse(data, "value", &result->value);
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockListItem> &object) {
    JsonWrapper json;
    if (object) {
        json.put("blocks", object->blocks);
        json.put("has_checkbox", object->hasCheckbox);
        json.put("is_checked", object->isChecked);
        json.put("value", object->value);
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
