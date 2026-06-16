#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockList.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockList> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockList>();
    parse(data, "type", &result->type);
    result->items = parseRequiredArray<RichBlockListItem>(data, "items");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockList> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("items", object->items);
    }
    return json;
}

} // namespace TgBot
