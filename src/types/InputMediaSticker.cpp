#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaSticker.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputMediaSticker) {
    auto result = std::make_shared<InputMediaSticker>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "emoji", &result->emoji);
    return result;
}

DECLARE_PARSER_TO_JSON(InputMediaSticker) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("emoji", object->emoji);
    }
    return json;
}

} // namespace TgBot
