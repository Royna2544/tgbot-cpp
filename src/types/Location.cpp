#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Location) {
    auto result = std::make_shared<Location>();
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "horizontal_accuracy", &result->horizontalAccuracy);
    parse(data, "live_period", &result->livePeriod);
    parse(data, "heading", &result->heading);
    parse(data, "proximity_alert_radius", &result->proximityAlertRadius);
    return result;
}

DECLARE_PARSER_TO_JSON(Location) {
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
