#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PassportElementErrorDataField) {
    auto result = std::make_shared<PassportElementErrorDataField>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    parse(data, "field_name", &result->fieldName);
    parse(data, "data_hash", &result->dataHash);
    parse(data, "message", &result->message);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorDataField) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        json.put("type", object->type);
        json.put("field_name", object->fieldName);
        json.put("data_hash", object->dataHash);
        json.put("message", object->message);
    }
    return json;
}

} // namespace TgBot
