#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ReactionTypeEmoji) {
    auto result = std::make_shared<ReactionTypeEmoji>();
    parse(data, "type", &result->type);
    parse(data, "emoji", &result->emoji);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionTypeEmoji) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("emoji", object->emoji);
    }
    return json;
}

} // namespace TgBot
