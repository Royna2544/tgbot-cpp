#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PassportElementErrorSelfie.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PassportElementErrorSelfie> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PassportElementErrorSelfie>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    parse(data, "file_hash", &result->fileHash);
    parse(data, "message", &result->message);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PassportElementErrorSelfie> &object) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        json.put("type", object->type);
        json.put("file_hash", object->fileHash);
        json.put("message", object->message);
    }
    return json;
}

} // namespace TgBot
