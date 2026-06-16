#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StoryAreaTypeLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StoryAreaTypeLink> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StoryAreaTypeLink>();
    parse(data, "type", &result->type);
    parse(data, "url", &result->url);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StoryAreaTypeLink> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
