#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BusinessMessagesDeleted) {
    auto result = std::make_shared<BusinessMessagesDeleted>();
    parse(data, "business_connection_id", &result->businessConnectionId);
    result->chat = parseRequired<Chat>(data, "chat");
    result->messageIds = parsePrimitiveRequiredArray<std::int32_t>(data, "message_ids");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessMessagesDeleted) {
    JsonWrapper json;
    if (object) {
        json.put("business_connection_id", object->businessConnectionId);
        json.put("chat", object->chat);
        json.put("message_ids", object->messageIds);
    }
    return json;
}

} // namespace TgBot
