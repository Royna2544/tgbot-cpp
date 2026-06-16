#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StoryAreaTypeSuggestedReaction.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StoryAreaTypeSuggestedReaction> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StoryAreaTypeSuggestedReaction>();
    parse(data, "type", &result->type);
    result->reactionType = parseRequired<ReactionType>(data, "reaction_type");
    parse(data, "is_dark", &result->isDark);
    parse(data, "is_flipped", &result->isFlipped);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StoryAreaTypeSuggestedReaction> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("reaction_type", object->reactionType);
        json.put("is_dark", object->isDark);
        json.put("is_flipped", object->isFlipped);
    }
    return json;
}

} // namespace TgBot
