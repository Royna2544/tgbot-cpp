#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(TransactionPartnerChat) {
    auto result = std::make_shared<TransactionPartnerChat>();
    parse(data, "type", &result->type);
    result->chat = parseRequired<Chat>(data, "chat");
    result->gift = parse<Gift>(data, "gift");
    return result;
}

DECLARE_PARSER_TO_JSON(TransactionPartnerChat) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("chat", object->chat);
        json.put("gift", object->gift);
    }
    return json;
}

} // namespace TgBot
