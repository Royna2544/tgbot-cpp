#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatShared) {
    auto result = std::make_shared<ChatShared>();
    parse(data, "request_id", &result->requestId);
    parse(data, "chat_id", &result->chatId);
    parse(data, "title", &result->title);
    parse(data, "username", &result->username);
    result->photo = parseArray<PhotoSize>(data, "photo");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatShared) {
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
