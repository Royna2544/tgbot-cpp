#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextEmailAddress.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextEmailAddress) {
    auto result = std::make_shared<RichTextEmailAddress>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "email_address", &result->emailAddress);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextEmailAddress) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("email_address", object->emailAddress);
    }
    return json;
}

} // namespace TgBot
