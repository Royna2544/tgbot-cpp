#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PassportData) {
    auto result = std::make_shared<PassportData>();
    result->data = parseRequiredArray<EncryptedPassportElement>(data, "data");
    result->credentials = parseRequired<EncryptedCredentials>(data, "credentials");
    return result;
}

DECLARE_PARSER_TO_JSON(PassportData) {
    JsonWrapper json;
    if (object) {
        json.put("data", object->data);
        json.put("credentials", object->credentials);
    }
    return json;
}

} // namespace TgBot
