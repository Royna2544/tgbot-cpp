#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageOrigin) {
    std::string type;
    MessageOrigin::Ptr result;
    parse(data, "type", &type);
    if (type == "user") {
        result = parse<MessageOriginUser>(data);
    } else if (type == "hidden_user") {
        result = parse<MessageOriginHiddenUser>(data);
    } else if (type == "chat") {
        result = parse<MessageOriginChat>(data);
    } else if (type == "channel") {
        result = parse<MessageOriginChannel>(data);
    } else {
        throw invalidType("MessageOrigin", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(MessageOrigin) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "user") {
            json += put<MessageOriginUser>(object);
        } else if (object->type == "hidden_user") {
            json += put<MessageOriginHiddenUser>(object);
        } else if (object->type == "chat") {
            json += put<MessageOriginChat>(object);
        } else if (object->type == "channel") {
            json += put<MessageOriginChannel>(object);
        } else {
            throw invalidType("MessageOrigin", object->type);
        }
    }
    return json;
}

} // namespace TgBot
