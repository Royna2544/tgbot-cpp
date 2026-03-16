#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatBoost) {
    auto result = std::make_shared<ChatBoost>();
    parse(data, "boost_id", &result->boostId);
    parse(data, "add_date", &result->addDate);
    parse(data, "expiration_date", &result->expirationDate);
    result->source = parseRequired<ChatBoostSource>(data, "source");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBoost) {
    JsonWrapper json;
    if (object) {
        json.put("boost_id", object->boostId);
        json.put("add_date", object->addDate);
        json.put("expiration_date", object->expirationDate);
        json.put("source", object->source);
    }
    return json;
}

} // namespace TgBot
