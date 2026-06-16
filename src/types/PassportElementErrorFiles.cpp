#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PassportElementErrorFiles.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PassportElementErrorFiles> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PassportElementErrorFiles>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    result->fileHashes = parsePrimitiveRequiredArray<std::string>(data, "file_hashes");
    parse(data, "message", &result->message);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PassportElementErrorFiles> &object) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        json.put("type", object->type);
        json.put("file_hashes", object->fileHashes);
        json.put("message", object->message);
    }
    return json;
}

} // namespace TgBot
