#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaLocation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputMediaLocation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputMediaLocation>();
    parse(data, "type", &result->type);
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "horizontal_accuracy", &result->horizontalAccuracy);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputMediaLocation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("horizontal_accuracy", object->horizontalAccuracy);
    }
    return json;
}

} // namespace TgBot
