#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(DirectMessagePriceChanged) {
    auto result = std::make_shared<DirectMessagePriceChanged>();
    parse(data, "are_direct_messages_enabled", &result->areDirectMessagesEnabled);
    parse(data, "direct_message_star_count", &result->directMessageStarCount);
    return result;
}

DECLARE_PARSER_TO_JSON(DirectMessagePriceChanged) {
    JsonWrapper json;
    if (object) {
        json.put("are_direct_messages_enabled", object->areDirectMessagesEnabled);
        json.put("direct_message_star_count", object->directMessageStarCount);
    }
    return json;
}

} // namespace TgBot
