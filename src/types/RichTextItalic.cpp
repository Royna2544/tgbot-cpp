#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextItalic.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextItalic) {
    auto result = std::make_shared<RichTextItalic>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextItalic) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
