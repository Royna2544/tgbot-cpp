#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatShared.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatShared> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatShared>();
    parse(data, "request_id", &result->requestId);
    parse(data, "chat_id", &result->chatId);
    parse(data, "title", &result->title);
    parse(data, "username", &result->username);
    result->photo = parseArray<PhotoSize>(data, "photo");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatShared> &object) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("chat_id", object->chatId);
        json.put("title", object->title);
        json.put("username", object->username);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
