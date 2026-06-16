#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StoryAreaTypeUniqueGift.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StoryAreaTypeUniqueGift> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StoryAreaTypeUniqueGift>();
    parse(data, "type", &result->type);
    parse(data, "name", &result->name);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StoryAreaTypeUniqueGift> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
