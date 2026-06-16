#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputLocationMessageContent.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputLocationMessageContent> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputLocationMessageContent>();
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "horizontal_accuracy", &result->horizontalAccuracy);
    parse(data, "live_period", &result->livePeriod);
    parse(data, "heading", &result->heading);
    parse(data, "proximity_alert_radius", &result->proximityAlertRadius);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputLocationMessageContent> &object) {
    JsonWrapper json;
    if (object) {
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("horizontal_accuracy", object->horizontalAccuracy);
        json.put("live_period", object->livePeriod);
        json.put("heading", object->heading);
        json.put("proximity_alert_radius", object->proximityAlertRadius);
    }
    return json;
}

} // namespace TgBot
