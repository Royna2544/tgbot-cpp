#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageId) {
    auto result = std::make_shared<MessageId>();
    parse(data, "message_id", &result->messageId);
    return result;
}

DECLARE_PARSER_TO_JSON(MessageId) {
    JsonWrapper json;
    if (object) {
        json.put("message_id", object->messageId);
    }
    return json;
}

} // namespace TgBot
