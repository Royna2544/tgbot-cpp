#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PassportData.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PassportData> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PassportData>();
    result->data = parseRequiredArray<EncryptedPassportElement>(data, "data");
    result->credentials = parseRequired<EncryptedCredentials>(data, "credentials");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PassportData> &object) {
    JsonWrapper json;
    if (object) {
        json.put("data", object->data);
        json.put("credentials", object->credentials);
    }
    return json;
}

} // namespace TgBot
