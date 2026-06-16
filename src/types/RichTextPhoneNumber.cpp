#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextPhoneNumber.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextPhoneNumber> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextPhoneNumber>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "phone_number", &result->phoneNumber);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextPhoneNumber> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("phone_number", object->phoneNumber);
    }
    return json;
}

} // namespace TgBot
