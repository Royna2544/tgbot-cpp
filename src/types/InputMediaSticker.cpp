#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaSticker.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputMediaSticker> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputMediaSticker>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "emoji", &result->emoji);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputMediaSticker> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("emoji", object->emoji);
    }
    return json;
}

} // namespace TgBot
