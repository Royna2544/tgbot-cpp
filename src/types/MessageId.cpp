#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MessageId.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MessageId> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MessageId>();
    parse(data, "message_id", &result->messageId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MessageId> &object) {
    JsonWrapper json;
    if (object) {
        json.put("message_id", object->messageId);
    }
    return json;
}

} // namespace TgBot
