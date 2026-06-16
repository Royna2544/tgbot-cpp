#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ForumTopicCreated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ForumTopicCreated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ForumTopicCreated>();
    parse(data, "name", &result->name);
    parse(data, "icon_color", &result->iconColor);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
    parse(data, "is_name_implicit", &result->isNameImplicit);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ForumTopicCreated> &object) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("icon_color", object->iconColor);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
        json.put("is_name_implicit", object->isNameImplicit);
    }
    return json;
}

} // namespace TgBot
