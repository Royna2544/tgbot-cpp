#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(KeyboardButton) {
    auto result = std::make_shared<KeyboardButton>();
    parse(data, "text", &result->text);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
    parse(data, "style", &result->style);
    result->requestUsers = parse<KeyboardButtonRequestUsers>(data, "request_users");
    result->requestChat = parse<KeyboardButtonRequestChat>(data, "request_chat");
    parse(data, "request_contact", &result->requestContact);
    parse(data, "request_location", &result->requestLocation);
    result->requestPoll = parse<KeyboardButtonPollType>(data, "request_poll");
    result->webApp = parse<WebAppInfo>(data, "web_app");
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButton) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
        json.put("style", object->style);
        json.put("request_users", object->requestUsers);
        json.put("request_chat", object->requestChat);
        json.put("request_contact", object->requestContact);
        json.put("request_location", object->requestLocation);
        json.put("request_poll", object->requestPoll);
        json.put("web_app", object->webApp);
    }
    return json;
}

} // namespace TgBot
