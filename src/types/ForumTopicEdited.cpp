#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ForumTopicEdited.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ForumTopicEdited> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ForumTopicEdited>();
    parse(data, "name", &result->name);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ForumTopicEdited> &object) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
    }
    return json;
}

} // namespace TgBot
