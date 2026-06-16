#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockPhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockPhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockPhoto>();
    parse(data, "type", &result->type);
    result->photo = parseRequiredArray<PhotoSize>(data, "photo");
    parse(data, "has_spoiler", &result->hasSpoiler);
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockPhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("photo", object->photo);
        json.put("has_spoiler", object->hasSpoiler);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
