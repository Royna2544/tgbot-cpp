#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotDescription.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotDescription> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotDescription>();
    parse(data, "description", &result->description);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotDescription> &object) {
    JsonWrapper json;
    if (object) {
        json.put("description", object->description);
    }
    return json;
}

} // namespace TgBot
