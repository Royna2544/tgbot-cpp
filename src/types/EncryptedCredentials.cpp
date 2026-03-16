#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(EncryptedCredentials) {
    auto result = std::make_shared<EncryptedCredentials>();
    parse(data, "data", &result->data);
    parse(data, "hash", &result->hash);
    parse(data, "secret", &result->secret);
    return result;
}

DECLARE_PARSER_TO_JSON(EncryptedCredentials) {
    JsonWrapper json;
    if (object) {
        json.put("data", object->data);
        json.put("hash", object->hash);
        json.put("secret", object->secret);
    }
    return json;
}

} // namespace TgBot
