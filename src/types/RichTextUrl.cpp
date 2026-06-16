#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextUrl.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextUrl) {
    auto result = std::make_shared<RichTextUrl>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "url", &result->url);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextUrl) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
