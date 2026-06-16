#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextMention.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextMention> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextMention>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "username", &result->username);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextMention> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("username", object->username);
    }
    return json;
}

} // namespace TgBot
