#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockMap.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockMap> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockMap>();
    parse(data, "type", &result->type);
    result->location = parseRequired<Location>(data, "location");
    parse(data, "zoom", &result->zoom);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockMap> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("location", object->location);
        json.put("zoom", object->zoom);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
