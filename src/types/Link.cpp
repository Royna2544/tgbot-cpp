#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Link.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Link> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Link>();
    parse(data, "url", &result->url);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Link> &object) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
