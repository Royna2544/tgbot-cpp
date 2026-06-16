#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StoryAreaTypeLocation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StoryAreaTypeLocation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StoryAreaTypeLocation>();
    parse(data, "type", &result->type);
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    result->address = parse<LocationAddress>(data, "address");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StoryAreaTypeLocation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("address", object->address);
    }
    return json;
}

} // namespace TgBot
