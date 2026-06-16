#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextReferenceLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextReferenceLink) {
    auto result = std::make_shared<RichTextReferenceLink>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "reference_name", &result->referenceName);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextReferenceLink) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("reference_name", object->referenceName);
    }
    return json;
}

} // namespace TgBot
