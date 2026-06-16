#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ReplyParameters.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ReplyParameters> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ReplyParameters>();
    parse(data, "message_id", &result->messageId);
    parse(data, "chat_id", &result->chatId);
    parse(data, "allow_sending_without_reply", &result->allowSendingWithoutReply);
    parse(data, "quote", &result->quote);
    parse(data, "quote_parse_mode", &result->quoteParseMode);
    result->quoteEntities = parseArray<MessageEntity>(data, "quote_entities");
    parse(data, "quote_position", &result->quotePosition);
    parse(data, "checklist_task_id", &result->checklistTaskId);
    parse(data, "poll_option_id", &result->pollOptionId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ReplyParameters> &object) {
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
        json.put("poll_option_id", object->pollOptionId);
    }
    return json;
}

} // namespace TgBot
