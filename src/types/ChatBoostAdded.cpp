#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatBoostAdded) {
    auto result = std::make_shared<ChatBoostAdded>();
    parse(data, "boost_count", &result->boostCount);
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBoostAdded) {
    JsonWrapper json;
    if (object) {
        json.put("boost_count", object->boostCount);
    }
    return json;
}

} // namespace TgBot
