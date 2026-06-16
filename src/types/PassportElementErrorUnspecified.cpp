#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PassportElementErrorUnspecified.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PassportElementErrorUnspecified> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PassportElementErrorUnspecified>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    parse(data, "element_hash", &result->elementHash);
    parse(data, "message", &result->message);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PassportElementErrorUnspecified> &object) {
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
