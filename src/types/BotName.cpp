#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotName.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotName> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotName>();
    parse(data, "name", &result->name);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotName> &object) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
