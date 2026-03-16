#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BusinessLocation) {
    auto result = std::make_shared<BusinessLocation>();
    parse(data, "address", &result->address);
    result->location = parse<Location>(data, "location");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessLocation) {
    JsonWrapper json;
    if (object) {
        json.put("address", object->address);
        json.put("location", object->location);
    }
    return json;
}

} // namespace TgBot
