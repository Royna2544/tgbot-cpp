#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PassportElementErrorTranslationFile) {
    auto result = std::make_shared<PassportElementErrorTranslationFile>();
    parse(data, "source", &result->source);
    parse(data, "type", &result->type);
    parse(data, "file_hash", &result->fileHash);
    parse(data, "message", &result->message);
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementErrorTranslationFile) {
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
