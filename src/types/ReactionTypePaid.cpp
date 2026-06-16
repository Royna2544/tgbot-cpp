#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ReactionTypePaid.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ReactionTypePaid> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ReactionTypePaid>();
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ReactionTypePaid> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
