#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockSectionHeading.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockSectionHeading> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockSectionHeading>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "size", &result->size);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockSectionHeading> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("size", object->size);
    }
    return json;
}

} // namespace TgBot
