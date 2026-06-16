#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockAnchor.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockAnchor> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockAnchor>();
    parse(data, "type", &result->type);
    parse(data, "name", &result->name);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockAnchor> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
