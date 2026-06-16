#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ReactionCount.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ReactionCount> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ReactionCount>();
    result->type = parseRequired<ReactionType>(data, "type");
    parse(data, "total_count", &result->totalCount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ReactionCount> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("total_count", object->totalCount);
    }
    return json;
}

} // namespace TgBot
