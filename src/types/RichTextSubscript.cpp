#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextSubscript.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextSubscript> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextSubscript>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextSubscript> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
