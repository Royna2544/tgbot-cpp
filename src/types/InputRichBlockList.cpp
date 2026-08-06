#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockList.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockList> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockList>();
    parse(data, "type", &result->type);
    result->items = parseRequiredArray<InputRichBlockListItem>(data, "items");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockList> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("items", object->items);
    }
    return json;
}

} // namespace TgBot
