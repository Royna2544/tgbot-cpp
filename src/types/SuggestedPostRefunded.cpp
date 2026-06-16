#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SuggestedPostRefunded.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SuggestedPostRefunded> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SuggestedPostRefunded>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    parse(data, "reason", &result->reason);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SuggestedPostRefunded> &object) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("reason", object->reason);
    }
    return json;
}

} // namespace TgBot
