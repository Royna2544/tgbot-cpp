#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextCode.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextCode) {
    auto result = std::make_shared<RichTextCode>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextCode) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
