#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PassportElementErrorDataField.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PassportElementErrorDataField> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PassportElementErrorDataField>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    parse(data, "field_name", &result->fieldName);
    parse(data, "data_hash", &result->dataHash);
    parse(data, "message", &result->message);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PassportElementErrorDataField> &object) {
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
