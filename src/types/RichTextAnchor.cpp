#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextAnchor.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextAnchor) {
    auto result = std::make_shared<RichTextAnchor>();
    parse(data, "type", &result->type);
    parse(data, "name", &result->name);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextAnchor) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
