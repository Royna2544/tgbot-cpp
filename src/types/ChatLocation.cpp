#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatLocation) {
    auto result = std::make_shared<ChatLocation>();
    result->location = parseRequired<Location>(data, "location");
    parse(data, "address", &result->address);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatLocation) {
    JsonWrapper json;
    if (object) {
        json.put("location", object->location);
        json.put("address", object->address);
    }
    return json;
}

} // namespace TgBot
