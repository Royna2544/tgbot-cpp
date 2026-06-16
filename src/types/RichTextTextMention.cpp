#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextTextMention.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextTextMention) {
    auto result = std::make_shared<RichTextTextMention>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    result->user = parseRequired<User>(data, "user");
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextTextMention) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
