#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextAnchorLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextAnchorLink) {
    auto result = std::make_shared<RichTextAnchorLink>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "anchor_name", &result->anchorName);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextAnchorLink) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("anchor_name", object->anchorName);
    }
    return json;
}

} // namespace TgBot
