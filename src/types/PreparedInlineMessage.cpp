#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PreparedInlineMessage) {
    auto result = std::make_shared<PreparedInlineMessage>();
    parse(data, "id", &result->id);
    parse(data, "expiration_date", &result->expirationDate);
    return result;
}

DECLARE_PARSER_TO_JSON(PreparedInlineMessage) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("expiration_date", object->expirationDate);
    }
    return json;
}

} // namespace TgBot
