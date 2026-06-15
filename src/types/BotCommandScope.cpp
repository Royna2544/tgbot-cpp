#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotCommandScope.h>
#include <tgbot/types/BotCommandScopeAllChatAdministrators.h>
#include <tgbot/types/BotCommandScopeAllGroupChats.h>
#include <tgbot/types/BotCommandScopeAllPrivateChats.h>
#include <tgbot/types/BotCommandScopeChat.h>
#include <tgbot/types/BotCommandScopeChatAdministrators.h>
#include <tgbot/types/BotCommandScopeChatMember.h>
#include <tgbot/types/BotCommandScopeDefault.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotCommandScope) {
    std::string type;
    BotCommandScope::Ptr result;
    parse(data, "type", &type);
    if (type == "default") {
        result = parse<BotCommandScopeDefault>(data);
    } else if (type == "all_private_chats") {
        result = parse<BotCommandScopeAllPrivateChats>(data);
    } else if (type == "all_group_chats") {
        result = parse<BotCommandScopeAllGroupChats>(data);
    } else if (type == "all_chat_administrators") {
        result = parse<BotCommandScopeAllChatAdministrators>(data);
    } else if (type == "chat") {
        result = parse<BotCommandScopeChat>(data);
    } else if (type == "chat_administrators") {
        result = parse<BotCommandScopeChatAdministrators>(data);
    } else if (type == "chat_member") {
        result = parse<BotCommandScopeChatMember>(data);
    } else {
        throw invalidType("BotCommandScope", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(BotCommandScope) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "default") {
            json += put<BotCommandScopeDefault>(object);
        } else if (object->type == "all_private_chats") {
            json += put<BotCommandScopeAllPrivateChats>(object);
        } else if (object->type == "all_group_chats") {
            json += put<BotCommandScopeAllGroupChats>(object);
        } else if (object->type == "all_chat_administrators") {
            json += put<BotCommandScopeAllChatAdministrators>(object);
        } else if (object->type == "chat") {
            json += put<BotCommandScopeChat>(object);
        } else if (object->type == "chat_administrators") {
            json += put<BotCommandScopeChatAdministrators>(object);
        } else if (object->type == "chat_member") {
            json += put<BotCommandScopeChatMember>(object);
        } else {
            throw invalidType("BotCommandScope", object->type);
        }
    }
    return json;
}

} // namespace TgBot
