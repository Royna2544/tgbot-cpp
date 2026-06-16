#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundFillFreeformGradient.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundFillFreeformGradient> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BackgroundFillFreeformGradient>();
    parse(data, "type", &result->type);
    result->colors = parsePrimitiveRequiredArray<std::int64_t>(data, "colors");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BackgroundFillFreeformGradient> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("colors", object->colors);
    }
    return json;
}

} // namespace TgBot
