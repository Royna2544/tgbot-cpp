#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockPreformatted.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockPreformatted> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockPreformatted>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "language", &result->language);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockPreformatted> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("language", object->language);
    }
    return json;
}

} // namespace TgBot
