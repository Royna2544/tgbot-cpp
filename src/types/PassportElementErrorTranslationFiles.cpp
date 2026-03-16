#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PassportElementErrorTranslationFiles) {
    auto result = std::make_shared<PassportElementErrorTranslationFiles>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    result->fileHashes = parsePrimitiveRequiredArray<std::string>(data, "file_hashes");
    parse(data, "message", &result->message);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorTranslationFiles) {
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
