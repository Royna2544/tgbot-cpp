#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PassportElementErrorUnspecified) {
    auto result = std::make_shared<PassportElementErrorUnspecified>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    parse(data, "element_hash", &result->elementHash);
    parse(data, "message", &result->message);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorUnspecified) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        json.put("type", object->type);
        json.put("element_hash", object->elementHash);
        json.put("message", object->message);
    }
    return json;
}

} // namespace TgBot
