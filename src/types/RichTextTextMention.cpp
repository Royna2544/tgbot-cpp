#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextTextMention.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextTextMention> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextTextMention>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    result->user = parseRequired<User>(data, "user");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextTextMention> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
