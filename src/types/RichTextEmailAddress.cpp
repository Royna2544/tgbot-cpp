#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextEmailAddress.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextEmailAddress> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextEmailAddress>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "email_address", &result->emailAddress);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextEmailAddress> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("email_address", object->emailAddress);
    }
    return json;
}

} // namespace TgBot
