#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InaccessibleMessage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InaccessibleMessage> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InaccessibleMessage>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    parse(data, "date", &result->date);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InaccessibleMessage> &object) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("message_id", object->messageId);
        json.put("date", object->date);
    }
    return json;
}

} // namespace TgBot
