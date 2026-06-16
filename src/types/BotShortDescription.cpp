#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotShortDescription.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotShortDescription> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotShortDescription>();
    parse(data, "short_description", &result->shortDescription);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotShortDescription> &object) {
    JsonWrapper json;
    if (object) {
        json.put("short_description", object->shortDescription);
    }
    return json;
}

} // namespace TgBot
