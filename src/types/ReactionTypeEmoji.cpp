#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ReactionTypeEmoji.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ReactionTypeEmoji> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ReactionTypeEmoji>();
    parse(data, "type", &result->type);
    parse(data, "emoji", &result->emoji);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ReactionTypeEmoji> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("emoji", object->emoji);
    }
    return json;
}

} // namespace TgBot
