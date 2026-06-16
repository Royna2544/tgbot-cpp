#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Dice.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Dice> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Dice>();
    parse(data, "emoji", &result->emoji);
    parse(data, "value", &result->value);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Dice> &object) {
    JsonWrapper json;
    if (object) {
        json.put("emoji", object->emoji);
        json.put("value", object->value);
    }
    return json;
}

} // namespace TgBot
