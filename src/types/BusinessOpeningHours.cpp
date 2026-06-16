#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BusinessOpeningHours.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BusinessOpeningHours> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BusinessOpeningHours>();
    parse(data, "time_zone_name", &result->timeZoneName);
    result->openingHours = parseRequiredArray<BusinessOpeningHoursInterval>(data, "opening_hours");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BusinessOpeningHours> &object) {
    JsonWrapper json;
    if (object) {
        json.put("time_zone_name", object->timeZoneName);
        json.put("opening_hours", object->openingHours);
    }
    return json;
}

} // namespace TgBot
