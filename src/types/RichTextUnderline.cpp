#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextUnderline.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextUnderline) {
    auto result = std::make_shared<RichTextUnderline>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextUnderline) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
