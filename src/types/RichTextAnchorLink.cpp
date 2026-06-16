#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextAnchorLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextAnchorLink> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextAnchorLink>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "anchor_name", &result->anchorName);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextAnchorLink> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("anchor_name", object->anchorName);
    }
    return json;
}

} // namespace TgBot
