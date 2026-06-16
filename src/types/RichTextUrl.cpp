#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextUrl.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextUrl> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextUrl>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "url", &result->url);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextUrl> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
