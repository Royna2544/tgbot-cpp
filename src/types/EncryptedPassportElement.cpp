#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(EncryptedPassportElement) {
    auto result = std::make_shared<EncryptedPassportElement>();
    parse(data, "type", &result->type);
    parse(data, "data", &result->data);
    parse(data, "phone_number", &result->phoneNumber);
    parse(data, "email", &result->email);
    result->files = parseArray<PassportFile>(data, "files");
    result->frontSide = parse<PassportFile>(data, "front_side");
    result->reverseSide = parse<PassportFile>(data, "reverse_side");
    result->selfie = parse<PassportFile>(data, "selfie");
    result->translation = parseArray<PassportFile>(data, "translation");
    parse(data, "hash", &result->hash);
    return result;
}

DECLARE_PARSER_TO_JSON(EncryptedPassportElement) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("data", object->data);
        json.put("phone_number", object->phoneNumber);
        json.put("email", object->email);
        json.put("files", object->files);
        json.put("front_side", object->frontSide);
        json.put("reverse_side", object->reverseSide);
        json.put("selfie", object->selfie);
        json.put("translation", object->translation);
        json.put("hash", object->hash);
    }
    return json;
}

} // namespace TgBot
