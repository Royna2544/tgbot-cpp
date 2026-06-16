#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockParagraph.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockParagraph> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockParagraph>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockParagraph> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
