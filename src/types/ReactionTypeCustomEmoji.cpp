#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ReactionTypeCustomEmoji) {
    auto result = std::make_shared<ReactionTypeCustomEmoji>();
    parse(data, "type", &result->type);
    parse(data, "custom_emoji_id", &result->customEmojiId);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionTypeCustomEmoji) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("custom_emoji_id", object->customEmojiId);
    }
    return json;
}

} // namespace TgBot
