#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ReactionTypeCustomEmoji.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ReactionTypeCustomEmoji> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ReactionTypeCustomEmoji>();
    parse(data, "type", &result->type);
    parse(data, "custom_emoji_id", &result->customEmojiId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ReactionTypeCustomEmoji> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("custom_emoji_id", object->customEmojiId);
    }
    return json;
}

} // namespace TgBot
