#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ProximityAlertTriggered.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ProximityAlertTriggered> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ProximityAlertTriggered>();
    result->traveler = parseRequired<User>(data, "traveler");
    result->watcher = parseRequired<User>(data, "watcher");
    parse(data, "distance", &result->distance);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ProximityAlertTriggered> &object) {
    JsonWrapper json;
    if (object) {
        json.put("traveler", object->traveler);
        json.put("watcher", object->watcher);
        json.put("distance", object->distance);
    }
    return json;
}

} // namespace TgBot
