#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BusinessMessagesDeleted.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BusinessMessagesDeleted> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BusinessMessagesDeleted>();
    parse(data, "business_connection_id", &result->businessConnectionId);
    result->chat = parseRequired<Chat>(data, "chat");
    result->messageIds = parsePrimitiveRequiredArray<std::int32_t>(data, "message_ids");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BusinessMessagesDeleted> &object) {
    JsonWrapper json;
    if (object) {
        json.put("business_connection_id", object->businessConnectionId);
        json.put("chat", object->chat);
        json.put("message_ids", object->messageIds);
    }
    return json;
}

} // namespace TgBot
