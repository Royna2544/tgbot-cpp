#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Gifts) {
    auto result = std::make_shared<Gifts>();
    result->gifts = parseRequiredArray<Gift>(data, "gifts");
    return result;
}

DECLARE_PARSER_TO_JSON(Gifts) {
    JsonWrapper json;
    if (object) {
        json.put("gifts", object->gifts);
    }
    return json;
}

} // namespace TgBot
