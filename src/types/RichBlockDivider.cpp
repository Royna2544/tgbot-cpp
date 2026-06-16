#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockDivider.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockDivider) {
    auto result = std::make_shared<RichBlockDivider>();
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockDivider) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
