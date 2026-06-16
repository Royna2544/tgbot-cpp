#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextReference.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextReference> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextReference>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "name", &result->name);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextReference> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
