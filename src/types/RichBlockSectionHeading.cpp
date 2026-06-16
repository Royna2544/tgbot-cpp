#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockSectionHeading.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockSectionHeading) {
    auto result = std::make_shared<RichBlockSectionHeading>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "size", &result->size);
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockSectionHeading) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("size", object->size);
    }
    return json;
}

} // namespace TgBot
