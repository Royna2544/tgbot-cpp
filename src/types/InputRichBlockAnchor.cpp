#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockAnchor.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockAnchor> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockAnchor>();
    parse(data, "type", &result->type);
    parse(data, "name", &result->name);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockAnchor> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
