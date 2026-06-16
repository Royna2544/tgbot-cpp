#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MenuButtonDefault.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MenuButtonDefault> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MenuButtonDefault>();
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MenuButtonDefault> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
