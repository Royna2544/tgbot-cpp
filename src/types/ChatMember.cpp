#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatMember) {
    std::string type;
    ChatMember::Ptr result;
    parse(data, "status", &type);
    if (type == "creator") {
        result = parse<ChatMemberOwner>(data);
    } else if (type == "administrator") {
        result = parse<ChatMemberAdministrator>(data);
    } else if (type == "member") {
        result = parse<ChatMemberMember>(data);
    } else if (type == "restricted") {
        result = parse<ChatMemberRestricted>(data);
    } else if (type == "left") {
        result = parse<ChatMemberLeft>(data);
    } else if (type == "kicked") {
        result = parse<ChatMemberBanned>(data);
    } else {
        throw invalidType("ChatMember", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(ChatMember) {
    JsonWrapper json;
    if (object) {
        json.put("status", object->status);
        if (object->status == "creator") {
            json += put<ChatMemberOwner>(object);
        } else if (object->status == "administrator") {
            json += put<ChatMemberAdministrator>(object);
        } else if (object->status == "member") {
            json += put<ChatMemberMember>(object);
        } else if (object->status == "restricted") {
            json += put<ChatMemberRestricted>(object);
        } else if (object->status == "left") {
            json += put<ChatMemberLeft>(object);
        } else if (object->status == "kicked") {
            json += put<ChatMemberBanned>(object);
        } else {
            throw invalidType("ChatMember", object->status);
        }
    }
    return json;
}

} // namespace TgBot
