#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextCustomEmoji.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextCustomEmoji) {
    auto result = std::make_shared<RichTextCustomEmoji>();
    parse(data, "type", &result->type);
    parse(data, "custom_emoji_id", &result->customEmojiId);
    parse(data, "alternative_text", &result->alternativeText);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextCustomEmoji) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("custom_emoji_id", object->customEmojiId);
        json.put("alternative_text", object->alternativeText);
    }
    return json;
}

} // namespace TgBot
