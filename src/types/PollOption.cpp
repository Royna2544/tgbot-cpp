#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PollOption) {
    auto result = std::make_shared<PollOption>();
    parse(data, "text", &result->text);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    parse(data, "voter_count", &result->voterCount);
    return result;
}

DECLARE_PARSER_TO_JSON(PollOption) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("text_entities", object->textEntities);
        json.put("voter_count", object->voterCount);
    }
    return json;
}

} // namespace TgBot
