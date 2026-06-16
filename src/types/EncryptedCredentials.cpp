#include <tgbot/TgTypeParser.h>
#include <tgbot/types/EncryptedCredentials.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<EncryptedCredentials> parse(const nlohmann::json &data) {
    auto result = std::make_shared<EncryptedCredentials>();
    parse(data, "data", &result->data);
    parse(data, "hash", &result->hash);
    parse(data, "secret", &result->secret);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<EncryptedCredentials> &object) {
    JsonWrapper json;
    if (object) {
        json.put("data", object->data);
        json.put("hash", object->hash);
        json.put("secret", object->secret);
    }
    return json;
}

} // namespace TgBot
