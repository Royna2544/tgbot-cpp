#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatJoinRequest.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatJoinRequest) {
    auto result = std::make_shared<ChatJoinRequest>();
    result->chat = parseRequired<Chat>(data, "chat");
    result->from = parseRequired<User>(data, "from");
    parse(data, "user_chat_id", &result->userChatId);
    parse(data, "date", &result->date);
    parse(data, "bio", &result->bio);
    result->inviteLink = parse<ChatInviteLink>(data, "invite_link");
    parse(data, "query_id", &result->queryId);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatJoinRequest) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("from", object->from);
        json.put("user_chat_id", object->userChatId);
        json.put("date", object->date);
        json.put("bio", object->bio);
        json.put("invite_link", object->inviteLink);
        json.put("query_id", object->queryId);
    }
    return json;
}

} // namespace TgBot
