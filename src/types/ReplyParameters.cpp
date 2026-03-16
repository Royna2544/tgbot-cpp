#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ReplyParameters) {
    auto result = std::make_shared<ReplyParameters>();
    parse(data, "message_id", &result->messageId);
    parse(data, "chat_id", &result->chatId);
    parse(data, "allow_sending_without_reply", &result->allowSendingWithoutReply);
    parse(data, "quote", &result->quote);
    parse(data, "quote_parse_mode", &result->quoteParseMode);
    result->quoteEntities = parseArray<MessageEntity>(data, "quote_entities");
    parse(data, "quote_position", &result->quotePosition);
    parse(data, "checklist_task_id", &result->checklistTaskId);
    return result;
}

DECLARE_PARSER_TO_JSON(ReplyParameters) {
    JsonWrapper json;
    if (object) {
        json.put("message_id", object->messageId);
        json.put("chat_id", object->chatId);
        json.put("allow_sending_without_reply", object->allowSendingWithoutReply);
        json.put("quote", object->quote);
        json.put("quote_parse_mode", object->quoteParseMode);
        json.put("quote_entities", object->quoteEntities);
        json.put("quote_position", object->quotePosition);
        json.put("checklist_task_id", object->checklistTaskId);
    }
    return json;
}

} // namespace TgBot
