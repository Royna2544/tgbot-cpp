#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PassportElementError) {
    std::string source;
    PassportElementError::Ptr result;
    parse(data, "source", &source);
    if (source == "data") {
        result = parse<PassportElementErrorDataField>(data);
    } else if (source == "front_side") {
        result = parse<PassportElementErrorFrontSide>(data);
    } else if (source == "reverse_side") {
        result = parse<PassportElementErrorReverseSide>(data);
    } else if (source == "selfie") {
        result = parse<PassportElementErrorSelfie>(data);
    } else if (source == "file") {
        result = parse<PassportElementErrorFile>(data);
    } else if (source == "files") {
        result = parse<PassportElementErrorFiles>(data);
    } else if (source == "translation_file") {
        result = parse<PassportElementErrorTranslationFile>(data);
    } else if (source == "translation_files") {
        result = parse<PassportElementErrorTranslationFiles>(data);
    } else if (source == "unspecified") {
        result = parse<PassportElementErrorUnspecified>(data);
    } else {
        throw invalidType("PassportElementError", source);
    }
    return result;
}

DECLARE_PARSER_TO_JSON(PassportElementError) {
    JsonWrapper json;
    if (object) {
        if (object->source == "data") {
            json += put<PassportElementErrorDataField>(object);
        } else if (object->source == "front_side") {
            json += put<PassportElementErrorFrontSide>(object);
        } else if (object->source == "reverse_side") {
            json += put<PassportElementErrorReverseSide>(object);
        } else if (object->source == "selfie") {
            json += put<PassportElementErrorSelfie>(object);
        } else if (object->source == "file") {
            json += put<PassportElementErrorFile>(object);
        } else if (object->source == "files") {
            json += put<PassportElementErrorFiles>(object);
        } else if (object->source == "translation_file") {
            json += put<PassportElementErrorTranslationFile>(object);
        } else if (object->source == "translation_files") {
            json += put<PassportElementErrorTranslationFiles>(object);
        } else if (object->source == "unspecified") {
            json += put<PassportElementErrorUnspecified>(object);
        } else {
            throw invalidType("PassportElementError", object->source);
        }
    }
    return json;
}

} // namespace TgBot
