#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockSlideshow.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockSlideshow> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockSlideshow>();
    parse(data, "type", &result->type);
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockSlideshow> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("blocks", object->blocks);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
