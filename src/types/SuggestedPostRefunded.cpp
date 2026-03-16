#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostRefunded) {
    auto result = std::make_shared<SuggestedPostRefunded>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    parse(data, "reason", &result->reason);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostRefunded) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("reason", object->reason);
    }
    return json;
}

} // namespace TgBot
