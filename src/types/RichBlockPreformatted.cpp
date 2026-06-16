#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockPreformatted.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockPreformatted) {
    auto result = std::make_shared<RichBlockPreformatted>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "language", &result->language);
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockPreformatted) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("language", object->language);
    }
    return json;
}

} // namespace TgBot
