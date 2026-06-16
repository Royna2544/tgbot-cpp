#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PollOptionDeleted.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PollOptionDeleted) {
    auto result = std::make_shared<PollOptionDeleted>();
    if (data.contains("poll_message") && !data["poll_message"].is_null()) {
        result->pollMessage = parse(data["poll_message"]);
    }
    parse(data, "option_persistent_id", &result->optionPersistentId);
    parse(data, "option_text", &result->optionText);
    result->optionTextEntities = parseArray<MessageEntity>(data, "option_text_entities");
    return result;
}

DECLARE_PARSER_TO_JSON(PollOptionDeleted) {
    JsonWrapper json;
    if (object) {
        json.put("poll_message", object->pollMessage);
        json.put("option_persistent_id", object->optionPersistentId);
        json.put("option_text", object->optionText);
        json.put("option_text_entities", object->optionTextEntities);
    }
    return json;
}

} // namespace TgBot
