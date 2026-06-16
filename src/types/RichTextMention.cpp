#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextMention.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextMention) {
    auto result = std::make_shared<RichTextMention>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "username", &result->username);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextMention) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("username", object->username);
    }
    return json;
}

} // namespace TgBot
