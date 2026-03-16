#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SwitchInlineQueryChosenChat) {
    auto result = std::make_shared<SwitchInlineQueryChosenChat>();
    parse(data, "query", &result->query);
    parse(data, "allow_user_chats", &result->allowUserChats);
    parse(data, "allow_bot_chats", &result->allowBotChats);
    parse(data, "allow_group_chats", &result->allowGroupChats);
    parse(data, "allow_channel_chats", &result->allowChannelChats);
    return result;
}

DECLARE_PARSER_TO_JSON(SwitchInlineQueryChosenChat) {
    JsonWrapper json;
    if (object) {
        json.put("query", object->query);
        json.put("allow_user_chats", object->allowUserChats);
        json.put("allow_bot_chats", object->allowBotChats);
        json.put("allow_group_chats", object->allowGroupChats);
        json.put("allow_channel_chats", object->allowChannelChats);
    }
    return json;
}

} // namespace TgBot
