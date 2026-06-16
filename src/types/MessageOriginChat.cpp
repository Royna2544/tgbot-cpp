#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MessageOriginChat.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MessageOriginChat> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MessageOriginChat>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    result->senderChat = parseRequired<Chat>(data, "sender_chat");
    parse(data, "author_signature", &result->authorSignature);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MessageOriginChat> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("sender_chat", object->senderChat);
        json.put("author_signature", object->authorSignature);
    }
    return json;
}

} // namespace TgBot
