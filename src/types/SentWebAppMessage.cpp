#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SentWebAppMessage) {
    auto result = std::make_shared<SentWebAppMessage>();
    parse(data, "inline_message_id", &result->inlineMessageId);
    return result;
}

DECLARE_PARSER_TO_JSON(SentWebAppMessage) {
    JsonWrapper json;
    if (object) {
        json.put("inline_message_id", object->inlineMessageId);
    }
    return json;
}

} // namespace TgBot
