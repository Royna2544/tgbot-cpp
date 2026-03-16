#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ProximityAlertTriggered) {
    auto result = std::make_shared<ProximityAlertTriggered>();
    result->traveler = parseRequired<User>(data, "traveler");
    result->watcher = parseRequired<User>(data, "watcher");
    parse(data, "distance", &result->distance);
    return result;
}

DECLARE_PARSER_TO_JSON(ProximityAlertTriggered) {
    JsonWrapper json;
    if (object) {
        json.put("traveler", object->traveler);
        json.put("watcher", object->watcher);
        json.put("distance", object->distance);
    }
    return json;
}

} // namespace TgBot
