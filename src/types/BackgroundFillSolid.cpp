#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundFillSolid.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundFillSolid> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BackgroundFillSolid>();
    parse(data, "type", &result->type);
    parse(data, "color", &result->color);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BackgroundFillSolid> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("color", object->color);
    }
    return json;
}

} // namespace TgBot
