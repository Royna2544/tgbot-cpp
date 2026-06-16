#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PassportElementError.h>
#include <tgbot/types/PassportElementErrorDataField.h>
#include <tgbot/types/PassportElementErrorFile.h>
#include <tgbot/types/PassportElementErrorFiles.h>
#include <tgbot/types/PassportElementErrorFrontSide.h>
#include <tgbot/types/PassportElementErrorReverseSide.h>
#include <tgbot/types/PassportElementErrorSelfie.h>
#include <tgbot/types/PassportElementErrorTranslationFile.h>
#include <tgbot/types/PassportElementErrorTranslationFiles.h>
#include <tgbot/types/PassportElementErrorUnspecified.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PassportElementError> parse(const nlohmann::json &data) {
    std::string type;
    PassportElementError::Ptr result;
    parse(data, "source", &type);
    if (type == "data") {
        result = parse<PassportElementErrorDataField>(data);
    } else if (type == "front_side") {
        result = parse<PassportElementErrorFrontSide>(data);
    } else if (type == "reverse_side") {
        result = parse<PassportElementErrorReverseSide>(data);
    } else if (type == "selfie") {
        result = parse<PassportElementErrorSelfie>(data);
    } else if (type == "file") {
        result = parse<PassportElementErrorFile>(data);
    } else if (type == "files") {
        result = parse<PassportElementErrorFiles>(data);
    } else if (type == "translation_file") {
        result = parse<PassportElementErrorTranslationFile>(data);
    } else if (type == "translation_files") {
        result = parse<PassportElementErrorTranslationFiles>(data);
    } else if (type == "unspecified") {
        result = parse<PassportElementErrorUnspecified>(data);
    } else {
        throw invalidType("PassportElementError", type);
    }

    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PassportElementError> &object) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
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
