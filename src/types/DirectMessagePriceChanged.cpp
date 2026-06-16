#include <tgbot/TgTypeParser.h>
#include <tgbot/types/DirectMessagePriceChanged.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<DirectMessagePriceChanged> parse(const nlohmann::json &data) {
    auto result = std::make_shared<DirectMessagePriceChanged>();
    parse(data, "are_direct_messages_enabled", &result->areDirectMessagesEnabled);
    parse(data, "direct_message_star_count", &result->directMessageStarCount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<DirectMessagePriceChanged> &object) {
    JsonWrapper json;
    if (object) {
        json.put("are_direct_messages_enabled", object->areDirectMessagesEnabled);
        json.put("direct_message_star_count", object->directMessageStarCount);
    }
    return json;
}

} // namespace TgBot
