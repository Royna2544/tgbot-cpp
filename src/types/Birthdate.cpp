#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Birthdate) {
    auto result = std::make_shared<Birthdate>();
    parse(data, "day", &result->day);
    parse(data, "month", &result->month);
    parse(data, "year", &result->year);
    return result;
}

DECLARE_PARSER_TO_JSON(Birthdate) {
    JsonWrapper json;
    if (object) {
        json.put("day", object->day);
        json.put("month", object->month);
        json.put("year", object->year);
    }
    return json;
}

} // namespace TgBot
