#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Chat) {
    auto result = std::make_shared<Chat>();
    parse(data, "id", &result->id);
    std::string type;
    parse(data, "type", &type);
    if (type == "private") {
        result->type = Chat::Type::Private;
    } else if (type == "group") {
        result->type = Chat::Type::Group;
    } else if (type == "supergroup") {
        result->type = Chat::Type::Supergroup;
    } else if (type == "channel") {
        result->type = Chat::Type::Channel;
    }

    parse(data, "title", &result->title);
    parse(data, "username", &result->username);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "is_forum", &result->isForum);
    parse(data, "is_direct_messages", &result->isDirectMessages);
    return result;
}

DECLARE_PARSER_TO_JSON(Chat) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        switch (object->type) {
            case Chat::Type::Private: json.put("type", "private"); break;
            case Chat::Type::Group: json.put("type", "group"); break;
            case Chat::Type::Supergroup: json.put("type", "supergroup"); break;
            case Chat::Type::Channel: json.put("type", "channel"); break;
        }
        json.put("title", object->title);
        json.put("username", object->username);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("is_forum", object->isForum);
        json.put("is_direct_messages", object->isDirectMessages);
    }
    return json;
}

} // namespace TgBot
