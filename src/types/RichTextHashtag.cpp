#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextHashtag.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextHashtag) {
    auto result = std::make_shared<RichTextHashtag>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "hashtag", &result->hashtag);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextHashtag) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("hashtag", object->hashtag);
    }
    return json;
}

} // namespace TgBot
