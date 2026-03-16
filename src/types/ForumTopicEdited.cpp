#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ForumTopicEdited) {
    auto result = std::make_shared<ForumTopicEdited>();
    parse(data, "name", &result->name);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopicEdited) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
    }
    return json;
}

} // namespace TgBot
