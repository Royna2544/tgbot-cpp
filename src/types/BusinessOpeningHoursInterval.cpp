#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BusinessOpeningHoursInterval) {
    auto result = std::make_shared<BusinessOpeningHoursInterval>();
    parse(data, "opening_minute", &result->openingMinute);
    parse(data, "closing_minute", &result->closingMinute);
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessOpeningHoursInterval) {
    JsonWrapper json;
    if (object) {
        json.put("opening_minute", object->openingMinute);
        json.put("closing_minute", object->closingMinute);
    }
    return json;
}

} // namespace TgBot
