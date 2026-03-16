#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ReactionType) {
    std::string type;
    ReactionType::Ptr result;
    parse(data, "type", &type);
    if (type == "emoji") {
        result = parse<ReactionTypeEmoji>(data);
    } else if (type == "custom_emoji") {
        result = parse<ReactionTypeCustomEmoji>(data);
    } else if (type == "paid") {
        result = parse<ReactionTypePaid>(data);
    } else {
        throw invalidType("ReactionType", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(ReactionType) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "emoji") {
            json += put<ReactionTypeEmoji>(object);
        } else if (object->type == "custom_emoji") {
            json += put<ReactionTypeCustomEmoji>(object);
        } else if (object->type == "paid") {
            json += put<ReactionTypePaid>(object);
        } else {
            throw invalidType("ReactionType", object->type);
        }
    }
    return json;
}

} // namespace TgBot
