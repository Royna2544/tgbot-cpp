#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextReference.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextReference) {
    auto result = std::make_shared<RichTextReference>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "name", &result->name);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextReference) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
