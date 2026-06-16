#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SentGuestMessage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SentGuestMessage) {
    auto result = std::make_shared<SentGuestMessage>();
    parse(data, "inline_message_id", &result->inlineMessageId);
    return result;
}

DECLARE_PARSER_TO_JSON(SentGuestMessage) {
    JsonWrapper json;
    if (object) {
        json.put("inline_message_id", object->inlineMessageId);
    }
    return json;
}

} // namespace TgBot
