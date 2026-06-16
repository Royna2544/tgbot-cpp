#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextReferenceLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextReferenceLink> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextReferenceLink>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "reference_name", &result->referenceName);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextReferenceLink> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("reference_name", object->referenceName);
    }
    return json;
}

} // namespace TgBot
