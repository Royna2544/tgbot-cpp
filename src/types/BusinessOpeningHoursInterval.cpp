#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BusinessOpeningHoursInterval.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BusinessOpeningHoursInterval> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BusinessOpeningHoursInterval>();
    parse(data, "opening_minute", &result->openingMinute);
    parse(data, "closing_minute", &result->closingMinute);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BusinessOpeningHoursInterval> &object) {
    JsonWrapper json;
    if (object) {
        json.put("opening_minute", object->openingMinute);
        json.put("closing_minute", object->closingMinute);
    }
    return json;
}

} // namespace TgBot
