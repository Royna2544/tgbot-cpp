#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Birthdate.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Birthdate> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Birthdate>();
    parse(data, "day", &result->day);
    parse(data, "month", &result->month);
    parse(data, "year", &result->year);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Birthdate> &object) {
    JsonWrapper json;
    if (object) {
        json.put("day", object->day);
        json.put("month", object->month);
        json.put("year", object->year);
    }
    return json;
}

} // namespace TgBot
