#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(LocationAddress) {
    auto result = std::make_shared<LocationAddress>();
    parse(data, "country_code", &result->countryCode);
    parse(data, "state", &result->state);
    parse(data, "city", &result->city);
    parse(data, "street", &result->street);
    return result;
}

DECLARE_PARSER_TO_JSON(LocationAddress) {
    JsonWrapper json;
    if (object) {
        json.put("country_code", object->countryCode);
        json.put("state", object->state);
        json.put("city", object->city);
        json.put("street", object->street);
    }
    return json;
}

} // namespace TgBot
