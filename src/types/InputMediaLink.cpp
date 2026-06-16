#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputMediaLink> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputMediaLink>();
    parse(data, "type", &result->type);
    parse(data, "url", &result->url);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputMediaLink> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
