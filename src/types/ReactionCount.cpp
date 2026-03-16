#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ReactionCount) {
    auto result = std::make_shared<ReactionCount>();
    result->type = parseRequired<ReactionType>(data, "type");
    parse(data, "total_count", &result->totalCount);
    return result;
}

DECLARE_PARSER_TO_JSON(ReactionCount) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("total_count", object->totalCount);
    }
    return json;
}

} // namespace TgBot
