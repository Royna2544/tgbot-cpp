#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BusinessOpeningHours) {
    auto result = std::make_shared<BusinessOpeningHours>();
    parse(data, "time_zone_name", &result->timeZoneName);
    result->openingHours = parseRequiredArray<BusinessOpeningHoursInterval>(data, "opening_hours");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessOpeningHours) {
    JsonWrapper json;
    if (object) {
        json.put("time_zone_name", object->timeZoneName);
        json.put("opening_hours", object->openingHours);
    }
    return json;
}

} // namespace TgBot
