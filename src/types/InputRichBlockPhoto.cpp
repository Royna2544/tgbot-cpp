#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockPhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockPhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockPhoto>();
    parse(data, "type", &result->type);
    result->photo = parseRequired<InputMediaPhoto>(data, "photo");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockPhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("photo", object->photo);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
