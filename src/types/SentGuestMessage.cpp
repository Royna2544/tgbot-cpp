#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SentGuestMessage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SentGuestMessage> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SentGuestMessage>();
    parse(data, "inline_message_id", &result->inlineMessageId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SentGuestMessage> &object) {
    JsonWrapper json;
    if (object) {
        json.put("inline_message_id", object->inlineMessageId);
    }
    return json;
}

} // namespace TgBot
