#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextSubscript.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextSubscript) {
    auto result = std::make_shared<RichTextSubscript>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextSubscript) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
