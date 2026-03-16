#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatBoostSourceGiftCode) {
    auto result = std::make_shared<ChatBoostSourceGiftCode>();
    parse(data, "source", &result->source);
    result->user = parseRequired<User>(data, "user");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBoostSourceGiftCode) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
